#include "image.h"

#include <SFML/Graphics/Image.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

static bool validBuffer(const ImageBuffer& img)
{
    if (img.width <= 0 || img.height <= 0)
        return false;
    const std::size_t n = static_cast<std::size_t>(img.width) * static_cast<std::size_t>(img.height) * 4;
    return img.rgba.size() >= n;
}

bool loadImage(const std::string& path, ImageBuffer& out)
{
    if (path.empty())
        return false;

    sf::Image img;
    if (!img.loadFromFile(path))
        return false;

    const auto size = img.getSize();
    if (size.x == 0 || size.y == 0)
        return false;

    const std::uint8_t* pixels = img.getPixelsPtr();
    if (!pixels)
        return false;

    out.width = static_cast<int>(size.x);
    out.height = static_cast<int>(size.y);
    const std::size_t n = static_cast<std::size_t>(out.width) * static_cast<std::size_t>(out.height) * 4;
    out.rgba.assign(pixels, pixels + n);
    return true;
}

bool saveImage(const std::string& path, const ImageBuffer& img)
{
    if (path.empty())
        return false;
    if (!validBuffer(img))
        return false;

    sf::Image out(
        sf::Vector2u(static_cast<unsigned>(img.width), static_cast<unsigned>(img.height)),
        img.rgba.data());

    // sfml sam wybiera format po rozszerzeniu, na nieznanym zwraca false
    return out.saveToFile(path);
}

bool loadBmp(const std::string& path, ImageBuffer& out)
{
    return loadImage(path, out);
}

bool saveBmp(const std::string& path, const ImageBuffer& img)
{
    return saveImage(path, img);
}

ImageBuffer diff(const ImageBuffer& a, const ImageBuffer& b)
{
    ImageBuffer out;

    // jak rozmiary sie nie zgadzaja to nic nie licze
    if (!validBuffer(a) || !validBuffer(b))
        return out;
    if (a.width != b.width || a.height != b.height)
        return out;

    const std::size_t n = static_cast<std::size_t>(a.width) * static_cast<std::size_t>(a.height) * 4;

    out.width = a.width;
    out.height = a.height;
    out.rgba.resize(n);

    for (std::size_t i = 0; i < n; i += 4) {
        out.rgba[i] = static_cast<unsigned char>(
            std::abs(static_cast<int>(a.rgba[i]) - static_cast<int>(b.rgba[i])));
        out.rgba[i + 1] = static_cast<unsigned char>(
            std::abs(static_cast<int>(a.rgba[i + 1]) - static_cast<int>(b.rgba[i + 1])));
        out.rgba[i + 2] = static_cast<unsigned char>(
            std::abs(static_cast<int>(a.rgba[i + 2]) - static_cast<int>(b.rgba[i + 2])));
        out.rgba[i + 3] = 255;
    }

    return out;
}

ImageBuffer cropStitch(const ImageBuffer& a, const ImageBuffer& b,
                       int x, int y, int w, int h)
{
    ImageBuffer out;

    if (!validBuffer(a) || !validBuffer(b))
        return out;
    if (w <= 0 || h <= 0)
        return out;

    // przyciecie do obszaru ktory jest w obu obrazach
    const int x0 = std::max(x, 0);
    const int y0 = std::max(y, 0);
    const int x1 = std::min({x + w, a.width, b.width});
    const int y1 = std::min({y + h, a.height, b.height});
    if (x1 <= x0 || y1 <= y0)
        return out;

    const int cw = x1 - x0;
    const int ch = y1 - y0;

    out.width = cw * 2;
    out.height = ch;
    out.rgba.resize(static_cast<std::size_t>(out.width) * static_cast<std::size_t>(out.height) * 4);

    const std::size_t rowBytes = static_cast<std::size_t>(cw) * 4;
    for (int row = 0; row < ch; ++row) {
        const std::size_t srcA = (static_cast<std::size_t>(y0 + row) * a.width + x0) * 4;
        const std::size_t srcB = (static_cast<std::size_t>(y0 + row) * b.width + x0) * 4;
        const std::size_t dst = static_cast<std::size_t>(row) * out.width * 4;

        std::memcpy(&out.rgba[dst], &a.rgba[srcA], rowBytes);
        std::memcpy(&out.rgba[dst + rowBytes], &b.rgba[srcB], rowBytes);
    }

    return out;
}
