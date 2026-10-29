#include "image.h"

#include <SFML/Graphics/Image.hpp>

#include <cstdint>

bool loadBmp(const std::string& path, ImageBuffer& out)
{
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
