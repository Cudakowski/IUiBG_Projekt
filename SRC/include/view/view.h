#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <vector>
#include <cstdint>

//#include "../image/image.h" //  TODO: zmienić
struct ImageBuffer;

// struct ImageBuffer {
//     int width = 0;
//     int height = 0;
//     std::vector<unsigned char> rgba;
// };

enum class ZoomMode {
    Discrete,   // 50%, 100%, 200%, 400%
    Continuous  // płynny 
};

struct ViewState {
    double zoomL = 1.0;
    double zoomR = 1.0;
    sf::Vector2f offset{0.f, 0.f}; 
    ZoomMode mode = ZoomMode::Discrete;
    bool relativeScaleEnabled = true; 
};

class Viewport {
public:
    Viewport();

    bool setLeftImage(const ImageBuffer& buffer);
    bool setRightImage(const ImageBuffer& buffer);


    void handleEvent(const std::optional<sf::Event>& event, const sf::FloatRect& viewportArea);


    void render(sf::RenderTarget& target, const sf::FloatRect& viewportArea);

    // zoom
    void zoomIn();
    void zoomOut();
    void setZoom(double newZoom);
    void resetView();
    void setZoomMode(ZoomMode mode);
    void setRelativeScale(bool enabled);

    // Dostęp do stanu widoku (dla UI i dokumentacji)
    const ViewState& getState() const { return state_; }
    void setState(const ViewState& newState);

    // Zwraca prostokąt ROI we współrzędnych pikseli obrazu bazowego (gotowe dla cropStitch Emilii)
    std::optional<sf::IntRect> getRoiInImagePixels() const;
    void clearRoi();

private:
    ViewState state_;
    sf::FloatRect lastArea_{{0.f, 0.f}, {800.f, 600.f}};

    // zasoby obrazów
    sf::Texture texL_;
    sf::Texture texR_;
    std::optional<sf::Sprite> spriteL_;
    std::optional<sf::Sprite> spriteR_;
    sf::Vector2u sizeL_{0, 0};
    sf::Vector2u sizeR_{0, 0};

    // skala wyrównująca prawy obraz do lewego
    float relativeFactorR_ = 1.0f;

    // stan pan
    bool isPanning_{false};
    sf::Vector2f lastMousePos_{0.f, 0.f};

    // stan roi
    bool isSelectingRoi_{false};
    bool hasRoi_{false};
    bool roiStartedOnRightPanel_{false};
    sf::Vector2f roiStartScreen_{0.f, 0.f};
    sf::Vector2f roiEndScreen_{0.f, 0.f};

    // progi powiększenia
    const std::vector<double> discreteLevels_{0.25, 0.5, 1.0, 2.0, 4.0, 8.0};




    void recalculateRelativeScale();
    void clampOffset();
    sf::Vector2f screenToImageCoords(sf::Vector2f screenPos, bool isRightPanel) const;
    sf::Vector2f imageToScreenCoords(sf::Vector2f imagePos, bool isRightPanel) const;
};


