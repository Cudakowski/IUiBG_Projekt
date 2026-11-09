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
