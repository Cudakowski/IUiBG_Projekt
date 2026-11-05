# 6. Opracowanie i opis niezbędnych algorytmów

Poniżej opis części związanej z obrazami (wczytywanie, zapis, obraz różnicowy).
Zoom, pan i skalowanie względem drugiego zdjęcia są po stronie viewportu
i będą opisane osobno.

## Wczytywanie BMP

Funkcja `loadBmp` dostaje ścieżkę do pliku i strukturę `ImageBuffer`, którą
ma wypełnić.

Najpierw sprawdzamy, czy ścieżka nie jest pusta. Potem SFML (`sf::Image`)
próbuje otworzyć plik. Jak się nie uda (brak pliku, uszkodzony plik, zły
format), funkcja zwraca `false` i nie zostawia sensownego obrazu.

Jak wczytanie przejdzie, bierzemy szerokość i wysokość oraz wskaźnik do
pikseli RGBA z SFML i kopiujemy je do `ImageBuffer.rgba`. Dzięki temu
dalsza część programu (różnica, zapis, potem też GUI) nie zależy już od
wewnętrznej reprezentacji SFML, tylko od naszej struktury.

## Zapis BMP

`saveBmp` robi odwrotnie: z `ImageBuffer` buduje `sf::Image` i zapisuje
go na dysk (rozszerzenie `.bmp`).

Przed zapisem sprawdzamy, czy ścieżka nie jest pusta, czy szerokość
i wysokość są dodatnie oraz czy tablica `rgba` ma wystarczająco dużo
bajtów (`width * height * 4`). Jak coś się nie zgadza albo SFML nie
zapisze pliku, zwracamy `false`.

Tą samą funkcją zapisujemy zwykły obraz i wynik obrazu różnicowego.

## Obraz różnicowy

Funkcja `diff` dostaje dwa buffery tego samego rozmiaru i zwraca trzeci.

Dla każdego piksela (indeksy idą po 4 bajty: R, G, B, A):

- R_out = |R_a - R_b|
- G_out = |G_a - G_b|
- B_out = |B_a - B_b|
- A_out = 255

Wartość bezwzględna jest po to, żeby wynik nie wychodził ujemny
(kolory trzymamy jako bajty 0-255). Alfa na wyniku jest zawsze pełna,
nie odejmujemy kanału A.

Jak obrazy mają różne szerokości albo wysokości, albo któryś ma puste
/ zbyt krótkie dane, funkcja zwraca pusty `ImageBuffer` (zerowy rozmiar).
W wersji podstawowej zakładamy i tak ten sam rozmiar obu zdjęć; różne
rozmiary to rozszerzenie i najpierw skala względna po stronie wyświetlania.

Po policzeniu różnicy wynik zapisuje się zwykłym `saveBmp` do pliku BMP.
