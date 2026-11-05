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

static bool testDiffMemory()
{
    ImageBuffer a;
    a.width = 2;
    a.height = 1;
    a.rgba = {
        10, 20, 30, 255,
        40, 50, 60, 255,
    };

    ImageBuffer b = a;
    b.rgba[0] = 15;
    b.rgba[5] = 45;

    ImageBuffer d = diff(a, b);
    if (d.width != 2 || d.height != 1 || d.rgba.size() != 8) {
        std::cerr << "diff: zly rozmiar wyniku\n";
        return false;
    }
    if (d.rgba[0] != 5 || d.rgba[1] != 0 || d.rgba[2] != 0 || d.rgba[3] != 255) {
        std::cerr << "diff: zly pierwszy piksel\n";
        return false;
    }
    if (d.rgba[4] != 0 || d.rgba[5] != 5 || d.rgba[6] != 0 || d.rgba[7] != 255) {
        std::cerr << "diff: zly drugi piksel\n";
        return false;
    }

    ImageBuffer zero = diff(a, a);
    for (std::size_t i = 0; i < zero.rgba.size(); i += 4) {
        if (zero.rgba[i] != 0 || zero.rgba[i + 1] != 0 || zero.rgba[i + 2] != 0 || zero.rgba[i + 3] != 255) {
            std::cerr << "diff(a,a): powinno byc czarne z A=255\n";
            return false;
        }
    }

    std::cout << "ok diff na bufferach w pamieci\n";
    return true;
}

static bool testDiffSamples(const std::string& dir)
{
    ImageBuffer left, right;
    if (!loadBmp(dir + "/para_same_a.bmp", left) || !loadBmp(dir + "/para_same_b.bmp", right)) {
        std::cerr << "diff samples: nie wczytalo pary\n";
        return false;
    }

    ImageBuffer d = diff(left, right);
    if (d.width != left.width || d.height != left.height) {
        std::cerr << "diff samples: zly rozmiar\n";
        return false;
    }

    const std::string outPath = dir + "/_tmp_diff.bmp";
    if (!saveBmp(outPath, d)) {
        std::cerr << "diff samples: nie zapisalo wyniku\n";
        return false;
    }

    ImageBuffer again;
    if (!loadBmp(outPath, again) || again.width != d.width || again.height != d.height) {
        std::cerr << "diff samples: reload wyniku fail\n";
        return false;
    }

    // rozne wymiary, diff ma zwrocic pusto
    ImageBuffer small, big;
    if (!loadBmp(dir + "/para_diff_a.bmp", small) || !loadBmp(dir + "/para_diff_b.bmp", big)) {
        std::cerr << "diff samples: nie wczytalo pary roznej\n";
        return false;
    }
    ImageBuffer empty = diff(small, big);
    if (empty.width != 0 || empty.height != 0 || !empty.rgba.empty()) {
        std::cerr << "diff roznych rozmiarow powinien byc pusty\n";
        return false;
    }

    std::cout << "ok diff na sample + zapis BMP\n";
    return true;
}

static bool testEdgeCases(const std::string& dir)
{
    ImageBuffer junk;

    if (loadBmp("", junk)) {
        std::cerr << "pusta sciezka nie powinna sie wczytac\n";
        return false;
    }

    if (saveBmp("", junk)) {
        std::cerr << "zapis na pusta sciezke nie powinien przejsc\n";
        return false;
    }

    if (loadBmp(dir + "/nie_ma_takiego.bmp", junk)) {
        std::cerr << "load nieistniejacego nie powinien przejsc\n";
        return false;
    }

    // z .. w sciezce tez ok
    const std::string viaDots = dir + "/../samples/para_same_a.bmp";
    ImageBuffer via;
    if (!loadBmp(viaDots, via) || via.width <= 0) {
        std::cerr << "load przez sciezke z .. fail: " << viaDots << "\n";
        return false;
    }

    ImageBuffer empty;
    if (saveBmp(dir + "/_tmp_empty.bmp", empty)) {
        std::cerr << "save pustego buffera nie powinien przejsc\n";
        return false;
    }

    ImageBuffer shortBuf;
    shortBuf.width = 2;
    shortBuf.height = 2;
    shortBuf.rgba = {1, 2, 3, 4}; // za malo na 2x2
    if (saveBmp(dir + "/_tmp_short.bmp", shortBuf)) {
        std::cerr << "save za krotkiego rgba nie powinien przejsc\n";
        return false;
    }

    std::cout << "ok edge cases sciezek/bledow\n";
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

    if (!testDiffMemory())
        ok = false;
    if (!testDiffSamples(dir))
        ok = false;
    if (!testEdgeCases(dir))
        ok = false;

    return ok ? 0 : 1;
}

// cd SRC/build && cmake --build . --target test_bmp -j && ./test_bmp
