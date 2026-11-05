# 4. Analiza projektu

## c) Zdefiniowanie struktur danych (obraz)

Obraz po wczytaniu trzymamy w strukturze `ImageBuffer`. Ma trzy pola:

- `width` - szerokość w pikselach
- `height` - wysokość w pikselach
- `rgba` - `std::vector<unsigned char>` z kolejnymi bajtami pikseli

Jeden piksel to 4 bajty: R, G, B, A (wartości 0-255). W wektorze leżą
jeden po drugim, wierszami od góry. Długość tablicy powinna wyjść
`width * height * 4`.

Taką postać mają i obraz wczytany z BMP, i wynik `diff`. Dzięki temu
zapis i dalsze operacje na pikselach wyglądają tak samo.

Stan widoku (zoom, offset, tryb) jest osobną strukturą po stronie
viewportu i tu go nie opisujemy.
