#include "view.h"
#include "imgui.h"
#include <algorithm>
#include <cmath>

// Jeśli masz już plik nagłówkowy Emilii, usuń poniższą definicję i użyj #include
struct ImageBuffer {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> rgba;
};

Viewport::Viewport() = default;

bool Viewport::setLeftImage(const ImageBuffer& buffer)
{
    if (buffer.width <= 0 || buffer.height <= 0 || buffer.rgba.empty()) return false;

    sf::Vector2u size{static_cast<unsigned int>(buffer.width), static_cast<unsigned int>(buffer.height)};
    if (!texL_.resize(size)) return false;

    texL_.update(buffer.rgba.data());
    texL_.setSmooth(false);
    sizeL_ = size;

    spriteL_.emplace(texL_);
    // Ustawiamy origin na środek obrazka, żeby (0,0) offsetu było dokładnie na środku panelu
    spriteL_->setOrigin({sizeL_.x / 2.f, sizeL_.y / 2.f});

    recalculateRelativeScale();
    clampOffset();
    return true;
}

bool Viewport::setRightImage(const ImageBuffer& buffer)
{
    if (buffer.width <= 0 || buffer.height <= 0 || buffer.rgba.empty())
        return false;

    sf::Vector2u size{static_cast<unsigned int>(buffer.width), static_cast<unsigned int>(buffer.height)};
    if (!texR_.resize(size))
        return false;

    texR_.update(buffer.rgba.data());
    texR_.setSmooth(false);
    sizeR_ = size;

    spriteR_.emplace(texR_);
    spriteR_->setOrigin({sizeR_.x / 2.f, sizeR_.y / 2.f});

    recalculateRelativeScale();
    clampOffset();
    return true;
}

void Viewport::recalculateRelativeScale()
{
    if (state_.relativeScaleEnabled && sizeL_.x > 0 && sizeL_.y > 0 && sizeR_.x > 0 && sizeR_.y > 0)
    {
        // Dopasowujemy skalę prawego obrazu względem lewego (np. po szerokości)
        relativeFactorR_ = static_cast<float>(sizeL_.x) / static_cast<float>(sizeR_.x);
    }
    else
    {
        relativeFactorR_ = 1.0f;
    }
    state_.zoomR = state_.zoomL * relativeFactorR_;
}

void Viewport::setRelativeScale(bool enabled)
{
    state_.relativeScaleEnabled = enabled;
    recalculateRelativeScale();
    clampOffset();
}

void Viewport::setZoomMode(ZoomMode mode)
{
    state_.mode = mode;
    if (mode == ZoomMode::Discrete)
    {
        // Przyciągnij zoom do progu
        setZoom(state_.zoomL);
    }
}

void Viewport::setZoom(double newZoom)
{
    if (state_.mode == ZoomMode::Discrete)
    {
        double closest = discreteLevels_.front();
        double minDiff = std::abs(newZoom - closest);
        for (double lvl : discreteLevels_)
        {
            double diff = std::abs(newZoom - lvl);
            if (diff < minDiff)
            {
                minDiff = diff;
                closest = lvl;
            }
        }
        state_.zoomL = closest;
    }
    else
    {
        state_.zoomL = std::clamp(newZoom, 0.1, 16.0);
    }

    state_.zoomR = state_.zoomL * relativeFactorR_;
    clampOffset();
}

void Viewport::zoomIn()
{
    if (state_.mode == ZoomMode::Discrete)
    {
        for (double lvl : discreteLevels_)
        {
            if (lvl > state_.zoomL + 1e-5)
            {
                setZoom(lvl);
                return;
            }
        }
    }
    else
    {
        setZoom(state_.zoomL * 1.15);
    }
}

void Viewport::zoomOut()
{
    if (state_.mode == ZoomMode::Discrete)
    {
        for (auto it = discreteLevels_.rbegin(); it != discreteLevels_.rend(); ++it)
        {
            if (*it < state_.zoomL - 1e-5)
            {
                setZoom(*it);
                return;
            }
        }
    }
    else
    {
        setZoom(state_.zoomL / 1.15);
    }
}

void Viewport::resetView()
{
    state_.offset = {0.f, 0.f};
    setZoom(1.0);
    clearRoi();
}

void Viewport::setState(const ViewState& newState)
{
    state_ = newState;
    recalculateRelativeScale();
    clampOffset();
}

void Viewport::clampOffset()
{
    float panelWidth = lastArea_.size.x / 2.f;
    float panelHeight = lastArea_.size.y;

    float effW = std::max(sizeL_.x * static_cast<float>(state_.zoomL),
                          sizeR_.x * static_cast<float>(state_.zoomR));
    float effH = std::max(sizeL_.y * static_cast<float>(state_.zoomL),
                          sizeR_.y * static_cast<float>(state_.zoomR));

    float maxOffsetX = std::max(panelWidth * 0.5f, effW * 0.5f);
    float maxOffsetY = std::max(panelHeight * 0.5f, effH * 0.5f);

    state_.offset.x = std::clamp(state_.offset.x, -maxOffsetX, maxOffsetX);
    state_.offset.y = std::clamp(state_.offset.y, -maxOffsetY, maxOffsetY);
}

void Viewport::handleEvent(const std::optional<sf::Event>& event, const sf::FloatRect& viewportArea)
{
    lastArea_ = viewportArea;
    if (!event) return;

    // kółko myszy - zoom
    if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        if (ImGui::GetIO().WantCaptureMouse) return;

        sf::Vector2f pos = sf::Vector2f(wheel->position);
        if (viewportArea.contains(pos))
        {
            if (wheel->delta > 0) zoomIn();
            else if (wheel->delta < 0) zoomOut();
        }
    }
    // Left = Pan, Right = ROI
    else if (const auto* m = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (ImGui::GetIO().WantCaptureMouse) return;

        sf::Vector2f pos = sf::Vector2f(m->position);
        if (!viewportArea.contains(pos)) return;

        if (m->button == sf::Mouse::Button::Left)
        {
            isPanning_ = true;
            lastMousePos_ = pos;
        }
        else if (m->button == sf::Mouse::Button::Right)
        {
            isSelectingRoi_ = true;
            hasRoi_ = true;
            float midX = viewportArea.position.x + viewportArea.size.x / 2.f;
            roiStartedOnRightPanel_ = (pos.x >= midX);
            roiStartScreen_ = pos;
            roiEndScreen_ = pos;
        }
    }
    else if (const auto* m = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (m->button == sf::Mouse::Button::Left)
        {
            isPanning_ = false;
        }
        else if (m->button == sf::Mouse::Button::Right)
        {
            isSelectingRoi_ = false;
            if (std::abs(roiStartScreen_.x - roiEndScreen_.x) < 2.f ||
                std::abs(roiStartScreen_.y - roiEndScreen_.y) < 2.f)
            {
                hasRoi_ = false;
            }
        }
    }
    else if (const auto* m = event->getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f currentPos = sf::Vector2f(m->position);

        if (isPanning_)
        {
            state_.offset += (currentPos - lastMousePos_);
            lastMousePos_ = currentPos;
            clampOffset();
        }
        if (isSelectingRoi_)
        {
            float midX = viewportArea.position.x + viewportArea.size.x / 2.f;
            float minX = roiStartedOnRightPanel_ ? midX : viewportArea.position.x;
            float maxX = roiStartedOnRightPanel_ ? (viewportArea.position.x + viewportArea.size.x) : midX;
            float minY = viewportArea.position.y;
            float maxY = viewportArea.position.y + viewportArea.size.y;

            roiEndScreen_.x = std::clamp(currentPos.x, minX, maxX);
            roiEndScreen_.y = std::clamp(currentPos.y, minY, maxY);
        }
    }
}

sf::Vector2f Viewport::screenToImageCoords(sf::Vector2f screenPos, bool isRightPanel) const
{
    float panelW = lastArea_.size.x / 2.f;
    float panelH = lastArea_.size.y;

    sf::Vector2f panelCenter{
        lastArea_.position.x + (isRightPanel ? panelW * 1.5f : panelW * 0.5f),
        lastArea_.position.y + panelH * 0.5f
    };

    double zoom = isRightPanel ? state_.zoomR : state_.zoomL;
    sf::Vector2u imgSize = isRightPanel ? sizeR_ : sizeL_;
    if (zoom <= 0.0) zoom = 1.0;

    sf::Vector2f relativeToCenter = (screenPos - panelCenter - state_.offset) / static_cast<float>(zoom);
    sf::Vector2f imgCoords = relativeToCenter + sf::Vector2f{imgSize.x / 2.f, imgSize.y / 2.f};

    if (isRightPanel && relativeFactorR_ > 0.f)
    {
        imgCoords *= relativeFactorR_;
    }

    return imgCoords;
}

sf::Vector2f Viewport::imageToScreenCoords(sf::Vector2f imagePos, bool isRightPanel) const
{
    float panelW = lastArea_.size.x / 2.f;
    float panelH = lastArea_.size.y;

    sf::Vector2f panelCenter{
        lastArea_.position.x + (isRightPanel ? panelW * 1.5f : panelW * 0.5f),
        lastArea_.position.y + panelH * 0.5f
    };

    sf::Vector2f localPos = imagePos;
    if (isRightPanel && relativeFactorR_ > 0.f)
    {
        localPos /= relativeFactorR_;
    }

    double zoom = isRightPanel ? state_.zoomR : state_.zoomL;
    sf::Vector2u imgSize = isRightPanel ? sizeR_ : sizeL_;

    sf::Vector2f relativeToCenter = (localPos - sf::Vector2f{imgSize.x / 2.f, imgSize.y / 2.f}) * static_cast<float>(zoom);
    return panelCenter + state_.offset + relativeToCenter;
}

std::optional<sf::IntRect> Viewport::getRoiInImagePixels() const
{
    if (!hasRoi_ || sizeL_.x == 0 || sizeL_.y == 0)
        return std::nullopt;

    sf::Vector2f p1 = screenToImageCoords(roiStartScreen_, roiStartedOnRightPanel_);
    sf::Vector2f p2 = screenToImageCoords(roiEndScreen_, roiStartedOnRightPanel_);

    int x1 = static_cast<int>(std::floor(std::min(p1.x, p2.x)));
    int y1 = static_cast<int>(std::floor(std::min(p1.y, p2.y)));
    int x2 = static_cast<int>(std::ceil(std::max(p1.x, p2.x)));
    int y2 = static_cast<int>(std::ceil(std::max(p1.y, p2.y)));


    x1 = std::clamp(x1, 0, static_cast<int>(sizeL_.x));
    y1 = std::clamp(y1, 0, static_cast<int>(sizeL_.y));
    x2 = std::clamp(x2, 0, static_cast<int>(sizeL_.x));
    y2 = std::clamp(y2, 0, static_cast<int>(sizeL_.y));

    if (x2 <= x1 || y2 <= y1)
        return std::nullopt;

    return sf::IntRect({x1, y1}, {x2 - x1, y2 - y1});
}

void Viewport::clearRoi()
{
    hasRoi_ = false;
    isSelectingRoi_ = false;
}

void Viewport::render(sf::RenderTarget& target, const sf::FloatRect& viewportArea)
{
    lastArea_ = viewportArea;
    sf::View originalView = target.getView();
    sf::Vector2u targetSize = target.getSize();

    if (targetSize.x == 0 || targetSize.y == 0 || viewportArea.size.x <= 0.f || viewportArea.size.y <= 0.f)
        return;

    float halfW = viewportArea.size.x / 2.f;
    float h = viewportArea.size.y;

    // LEWY 
    sf::FloatRect leftArea({viewportArea.position.x, viewportArea.position.y}, {halfW, h});
    sf::View leftView(leftArea);
    leftView.setViewport(sf::FloatRect(
        {leftArea.position.x / targetSize.x, leftArea.position.y / targetSize.y},
        {leftArea.size.x / targetSize.x, leftArea.size.y / targetSize.y}
    ));
    target.setView(leftView);

    if (spriteL_)
    {
        sf::Vector2f centerL{leftArea.position.x + halfW / 2.f, leftArea.position.y + h / 2.f};
        spriteL_->setPosition(centerL + state_.offset);
        spriteL_->setScale({static_cast<float>(state_.zoomL), static_cast<float>(state_.zoomL)});
        target.draw(*spriteL_);
    }

    // PRAWY
    sf::FloatRect rightArea({viewportArea.position.x + halfW, viewportArea.position.y}, {halfW, h});
    sf::View rightView(rightArea);
    rightView.setViewport(sf::FloatRect(
        {rightArea.position.x / targetSize.x, rightArea.position.y / targetSize.y},
        {rightArea.size.x / targetSize.x, rightArea.size.y / targetSize.y}
    ));
    target.setView(rightView);

    if (spriteR_)
    {
        sf::Vector2f centerR{rightArea.position.x + halfW / 2.f, rightArea.position.y + h / 2.f};
        spriteR_->setPosition(centerR + state_.offset);
        spriteR_->setScale({static_cast<float>(state_.zoomR), static_cast<float>(state_.zoomR)});
        target.draw(*spriteR_);
    }

    //PIONOWA KRESKA PODZIAŁU I RAMKA ROI
    sf::View fullAreaView(viewportArea);
    fullAreaView.setViewport(sf::FloatRect(
        {viewportArea.position.x / targetSize.x, viewportArea.position.y / targetSize.y},
        {viewportArea.size.x / targetSize.x, viewportArea.size.y / targetSize.y}
    ));
    target.setView(fullAreaView);

    // linia
    sf::RectangleShape divider({2.f, h});
    divider.setPosition({viewportArea.position.x + halfW - 1.f, viewportArea.position.y});
    divider.setFillColor(sf::Color(200, 200, 200, 255));
    target.draw(divider);

    // Rysowanie prostokąta ROI po obu stronach
    if (auto roiPixels = getRoiInImagePixels())
    {
        sf::Vector2f topLeftImg{static_cast<float>(roiPixels->position.x), static_cast<float>(roiPixels->position.y)};
        sf::Vector2f botRightImg{static_cast<float>(roiPixels->position.x + roiPixels->size.x),
                                 static_cast<float>(roiPixels->position.y + roiPixels->size.y)};

        for (bool isRight : {false, true})
        {
            sf::Vector2f tl = imageToScreenCoords(topLeftImg, isRight);
            sf::Vector2f br = imageToScreenCoords(botRightImg, isRight);

            sf::RectangleShape roiBox(br - tl);
            roiBox.setPosition(tl);
            roiBox.setFillColor(sf::Color(0, 180, 255, 45));
            roiBox.setOutlineColor(sf::Color(0, 180, 255, 230));
            roiBox.setOutlineThickness(1.5f);
            target.draw(roiBox);
        }
    }


    target.setView(originalView);
}