#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <vector>
#include <cstdint>

//#include "../image/image.h"
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
    sf::Vector2f offset{0.f, 0.f}; // (0, 0) = wyśrodkowany względem panelu
    ZoomMode mode = ZoomMode::Discrete;
    bool relativeScaleEnabled = true; // Skala względna przy różnych rozmiarach obrazów
};

class Viewport {
public:
    Viewport();

    // Ustawienie lub aktualizacja obrazów z modułu ImageCore (Emilii)
    bool setLeftImage(const ImageBuffer& buffer);
    bool setRightImage(const ImageBuffer& buffer);

    // Obsługa zdarzeń SFML (mysz: pan, zoom kółkiem, zaznaczanie ROI)
    void handleEvent(const std::optional<sf::Event>& event, const sf::FloatRect& viewportArea);

    // Rysowanie split-view, obu tekstur, pionowej kreski oraz ramki ROI
    void render(sf::RenderTarget& target, const sf::FloatRect& viewportArea);

    // Sterowanie zoomem (dla przycisków/suwaków UI Krzysztofa)
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

    // Zasoby SFML dla obu obrazów
    sf::Texture texL_;
    sf::Texture texR_;
    std::optional<sf::Sprite> spriteL_;
    std::optional<sf::Sprite> spriteR_;
    sf::Vector2u sizeL_{0, 0};
    sf::Vector2u sizeR_{0, 0};

    // Skala bazowa wyrównująca rozmiar prawego obrazu do lewego
    float relativeFactorR_ = 1.0f;

    // Stan przesuwania (Pan)
    bool isPanning_{false};
    sf::Vector2f lastMousePos_{0.f, 0.f};

    // Stan zaznaczania prostokąta (ROI)
    bool isSelectingRoi_{false};
    bool hasRoi_{false};
    bool roiStartedOnRightPanel_{false};
    sf::Vector2f roiStartScreen_{0.f, 0.f};
    sf::Vector2f roiEndScreen_{0.f, 0.f};

    // Dyskretne progi powiększenia
    const std::vector<double> discreteLevels_{0.25, 0.5, 1.0, 2.0, 4.0, 8.0};

    // Metody pomocnicze
    void recalculateRelativeScale();
    void clampOffset();
    sf::Vector2f screenToImageCoords(sf::Vector2f screenPos, bool isRightPanel) const;
    sf::Vector2f imageToScreenCoords(sf::Vector2f imagePos, bool isRightPanel) const;
};


// /*
// Split view + kreska, zoom dyskretny/ciągły, pan sync, skala względna, zaznaczanie ROI


// */

// #include "imgui.h"
// #include "imgui-SFML.h"
// #include "ImGuiFileDialog.h"


// #include <iostream>
// #include <SFML/Window.hpp>
// #include <SFML/Graphics.hpp>

// enum Mode{

// };

// struct ViewState{ 
//     double zoomL;
//     double zoomR;
//     sf::Vector2u offset; //  (0, 0) = na środku
//     Mode mode; 
// };

// class Viewport {
// private:
//     // Stan przesuwania
//     bool isPanning_{false};
//     sf::Vector2f lastMousePos_{};
//     sf::Vector2f panOffset_{0.f, 0.f};

//     // Stan zaznaczania prostokąta
//     bool isSelectingRoi_{false};
//     bool hasRoi_{false};
//     sf::Vector2f roiStart_{};
//     sf::Vector2f roiEnd_{};

// public:
//     void handleEvent(const std::optional<sf::Event>& event);
//     std::optional<sf::FloatRect> getRoiRect() const; // Zwraca prostokąt, jeśli istnieje
// };

// void Viewport::handleEvent(const std::optional<sf::Event>& event)
// {
//     if (!event) return;

//     if (const auto* m = event->getIf<sf::Event::MouseButtonPressed>())
//     {
//         if (ImGui::GetIO().WantCaptureMouse) return;

//         sf::Vector2f pos = sf::Vector2f(m->position);

//         if (m->button == sf::Mouse::Button::Left) // Przesuwanie (Pan)
//         {
//             isPanning_ = true;
//             lastMousePos_ = pos;
//         }
//         else if (m->button == sf::Mouse::Button::Right) // Zaznaczanie ROI
//         {
//             isSelectingRoi_ = true;
//             hasRoi_ = true;
//             roiStart_ = pos;
//             roiEnd_ = pos;
//         }
//     }
//     else if (const auto* m = event->getIf<sf::Event::MouseButtonReleased>())
//     {
//         if (m->button == sf::Mouse::Button::Left)
//         {
//             isPanning_ = false;
//         }
//         else if (m->button == sf::Mouse::Button::Right)
//         {
//             isSelectingRoi_ = false;
//             // Opcjonalnie: jeśli użytkownik tylko kliknął (prostokąt 0x0), wyłącz ROI
//             if (roiStart_ == roiEnd_) hasRoi_ = false;
//         }
//     }
//     else if (const auto* m = event->getIf<sf::Event::MouseMoved>())
//     {
//         sf::Vector2f currentPos = sf::Vector2f(m->position);

//         if (isPanning_)
//         {
//             panOffset_ += (currentPos - lastMousePos_);
//             lastMousePos_ = currentPos;
//         }
//         if (isSelectingRoi_)
//         {
//             roiEnd_ = currentPos;
//         }
//     }
// }


// std::optional<sf::FloatRect> Viewport::getRoiRect() const
// {
//     if (!hasRoi_) return std::nullopt;

//     float left   = std::min(roiStart_.x, roiEnd_.x);
//     float top    = std::min(roiStart_.y, roiEnd_.y);
//     float width  = std::abs(roiEnd_.x - roiStart_.x);
//     float height = std::abs(roiEnd_.y - roiStart_.y);

//     return sf::FloatRect({left, top}, {width, height});
// }







/*
#pragma once

#include <string>
#include <vector>

// formaty: bmp, png, jpg (i co jeszcze sfml umie, np. tga)
// piksele: RGBA, po bajcie, kolejno w tablicy (rozmiar = w*h*4)

struct ImageBuffer {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> rgba;
};

// wczytanie obrazu (bmp/png/jpg), true jak sie uda
bool loadImage(const std::string& path, ImageBuffer& out);

// zapis, format po rozszerzeniu (.bmp/.png/.jpg/.tga)
bool saveImage(const std::string& path, const ImageBuffer& img);

// stare nazwy, zostaja zeby nic nie popsuc
bool loadBmp(const std::string& path, ImageBuffer& out);
bool saveBmp(const std::string& path, const ImageBuffer& img);

// obraz roznicowy |R|,|G|,|B|; na poczatek ten sam rozmiar
ImageBuffer diff(const ImageBuffer& a, const ImageBuffer& b);

// wycinek x,y,w,h z obu obrazow sklejony obok siebie (lewy | prawy)
// prostokat przycinany do czesci wspolnej, jak nic nie zostanie to pusty wynik
ImageBuffer cropStitch(const ImageBuffer& a, const ImageBuffer& b,
                       int x, int y, int w, int h);

*/