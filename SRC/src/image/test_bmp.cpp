#include "image.h"

#include <iostream>
#include <string>

#ifndef SAMPLES_DIR
#define SAMPLES_DIR "../assets/samples"
#endif

static bool samePixels(const ImageBuffer& a, const ImageBuffer& b)
{
    if (a.width != b.width || a.height != b.height)
        return false;
    if (a.rgba.size() != b.rgba.size())
        return false;
    return a.rgba == b.rgba;
}

static bool roundTrip(const std::string& path)
{
    ImageBuffer img;
    if (!loadBmp(path, img)) {
        std::cerr << "load fail: " << path << "\n";
        return false;
    }

    if (img.width <= 0 || img.height <= 0) {
        std::cerr << "zly rozmiar po load: " << path << "\n";
        return false;
    }

    const std::size_t expect = static_cast<std::size_t>(img.width) * static_cast<std::size_t>(img.height) * 4;
    if (img.rgba.size() != expect) {
        std::cerr << "zly rozmiar rgba: " << path << "\n";
        return false;
    }

    const std::string outPath = std::string(SAMPLES_DIR) + "/_tmp_roundtrip.bmp";
    if (!saveBmp(outPath, img)) {
        std::cerr << "save fail: " << outPath << "\n";
        return false;
    }

    ImageBuffer again;
    if (!loadBmp(outPath, again)) {
        std::cerr << "reload fail: " << outPath << "\n";
        return false;
    }

    if (!samePixels(img, again)) {
        std::cerr << "piksele sie nie zgadzaja po round-trip: " << path << "\n";
        return false;
    }

    std::cout << "ok " << path << " (" << img.width << "x" << img.height << ")\n";
    return true;
}

int main()
{
    const std::string dir = SAMPLES_DIR;
    const char* files[] = {
        "para_same_a.bmp",
        "para_same_b.bmp",
        "para_diff_a.bmp",
        "para_diff_b.bmp",
    };

    bool ok = true;
    for (const char* name : files) {
        if (!roundTrip(dir + "/" + name))
            ok = false;
    }

    // pusty buffer nie powinien sie zapisac
    ImageBuffer empty;
    if (saveBmp(dir + "/_tmp_empty.bmp", empty)) {
        std::cerr << "save pustego buffera nie powinien przejsc\n";
        ok = false;
    } else {
        std::cout << "ok save pustego odrzucony\n";
    }

    // nieistniejacy plik
    ImageBuffer junk;
    if (loadBmp(dir + "/nie_ma_takiego.bmp", junk)) {
        std::cerr << "load nieistniejacego nie powinien przejsc\n";
        ok = false;
    } else {
        std::cout << "ok load nieistniejacego odrzucony\n";
    }

    return ok ? 0 : 1;
}

/*
    odpalenie testu

    cd SRC/build
    cmake ..
    cmake --build . --target test_bmp -j
    ./test_bmp
*/