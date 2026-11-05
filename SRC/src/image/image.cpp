#include "image.h"

#include <SFML/Graphics/Image.hpp>

#include <cstdint>
#include <cstdlib>

bool loadBmp(const std::string& path, ImageBuffer& out)
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

bool saveBmp(const std::string& path, const ImageBuffer& img)
{
    if (path.empty())
        return false;
    if (img.width <= 0 || img.height <= 0)
        return false;

    const std::size_t n = static_cast<std::size_t>(img.width) * static_cast<std::size_t>(img.height) * 4;
    if (img.rgba.size() < n)
        return false;

    sf::Image out(
        sf::Vector2u(static_cast<unsigned>(img.width), static_cast<unsigned>(img.height)),
        img.rgba.data());

    return out.saveToFile(path);
}

ImageBuffer diff(const ImageBuffer& a, const ImageBuffer& b)
{
    ImageBuffer out;

    // jak rozmiary sie nie zgadzaja to nic nie licze
    if (a.width <= 0 || a.height <= 0)
        return out;
    if (a.width != b.width || a.height != b.height)
        return out;

    const std::size_t n = static_cast<std::size_t>(a.width) * static_cast<std::size_t>(a.height) * 4;
    if (a.rgba.size() < n || b.rgba.size() < n)
        return out;

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
