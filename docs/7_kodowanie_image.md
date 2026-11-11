# 7. Kodowanie (moduł obrazu)

Kod od obrazów siedzi w `SRC/include/image/image.h` i `SRC/src/image/image.cpp`.
Nie ma tu żadnego okna ani ImGui - tylko wczytywanie, zapis i operacje na
pikselach. Reszta programu (menu, viewport) woła te funkcje i dostaje gotowy
`ImageBuffer`.

## Struktura ImageBuffer

```
struct ImageBuffer {
    int width;
    int height;
    std::vector<unsigned char> rgba;
};
```

`width` i `height` to rozmiar w pikselach. `rgba` trzyma kolory po kolei:
dla każdego piksela cztery bajty R, G, B, A. Indeks początku piksela
`(x, y)` to `(y * width + x) * 4`. Jak obraz jest pusty, szerokość
i wysokość są zerowe, a wektor pusty.

## Funkcje

`loadImage(path, out)` - wczytuje plik przez SFML (`sf::Image::loadFromFile`).
Jak ścieżka jest pusta albo plik się nie otworzy, zwraca `false`. Po sukcesie
kopiuje rozmiar i piksele RGBA do `out`. Format (BMP, PNG, JPG, TGA) rozpoznaje
SFML sam.

`saveImage(path, img)` - z `ImageBuffer` robi `sf::Image` i zapisuje na dysk.
Format bierze z rozszerzenia w ścieżce. Przed zapisem sprawdza, czy buffer
ma sensowny rozmiar i czy `rgba` nie jest za krótkie. Jak coś nie gra albo
rozszerzenie jest nieznane, zwraca `false`.

`loadBmp` / `saveBmp` - to po prostu wywołania `loadImage` / `saveImage`.
Zostawione pod starą nazwą, żeby reszta kodu nie musiała się od razu
przerabiać.

`diff(a, b)` - liczy obraz różnicowy. Dla każdego piksela bierze wartość
bezwzględną różnicy R, G, B; A ustawia na 255. Jak obrazy mają różny rozmiar
albo któryś jest pusty / uszkodzony, zwraca pusty buffer.

`cropStitch(a, b, x, y, w, h)` - wycina prostokąt z obu obrazów i skleja
je obok siebie (lewy | prawy). Prostokąt jest najpierw przycinany do obszaru
wspólnego obu obrazów. Wynik ma szerokość `2 * cw` i wysokość `ch`. Kopiowanie
idzie wierszami (`memcpy`), nie piksel po pikselu. Jak po przycięciu nic
nie zostanie, wraca pusty buffer.

Wewnętrznie jest jeszcze `validBuffer` - sprawdza, czy `width`/`height` > 0
i czy `rgba` ma przynajmniej `width * height * 4` bajtów. Używają tego
zapis, diff i cropStitch.

## Zależności

Moduł linkuje SFML Graphics (to samo co reszta projektu z labów). Nie używa
OpenGL ani DirectX poza tym, co ImGui/SFML i tak ciągnie w aplikacji głównej.
Test obrazu (`test_bmp`) linkuje tylko SFML, bez ImGui.
