#pragma once

#include <string>
#include <vector>

// formaty: BMP (na razie), pozniej PNG/JPG
// piksele: RGBA, po bajcie, kolejno w tablicy (rozmiar = w*h*4)

struct ImageBuffer {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> rgba;
};

// wczytanie BMP, true jak sie uda
bool loadBmp(const std::string& path, ImageBuffer& out);

// zapis do BMP
bool saveBmp(const std::string& path, const ImageBuffer& img);

// obraz roznicowy |R|,|G|,|B|; na poczatek ten sam rozmiar
ImageBuffer diff(const ImageBuffer& a, const ImageBuffer& b);

// wycinek z obu obrazow i sklejenie obok siebie
ImageBuffer cropStitch(const ImageBuffer& a, const ImageBuffer& b,
                       int x, int y, int w, int h);
