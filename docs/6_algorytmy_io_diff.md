# 6. Opracowanie i opis niezbędnych algorytmów

Poniżej opis części związanej z obrazami (wczytywanie, zapis, obraz różnicowy,
wycinek ze sklejeniem). Zoom, pan i skalowanie względem drugiego zdjęcia są
po stronie viewportu i będą opisane osobno.

## Wczytywanie obrazu

Funkcja `loadImage` dostaje ścieżkę do pliku i strukturę `ImageBuffer`, którą
ma wypełnić. `loadBmp` zostało jako druga nazwa tej samej funkcji.

Najpierw sprawdzamy, czy ścieżka nie jest pusta. Potem SFML (`sf::Image`)
próbuje otworzyć plik. Jak się nie uda (brak pliku, uszkodzony plik, zły
format), funkcja zwraca `false` i nie zostawia sensownego obrazu.

Jak wczytanie przejdzie, bierzemy szerokość i wysokość oraz wskaźnik do
pikseli RGBA z SFML i kopiujemy je do `ImageBuffer.rgba`. Dzięki temu
dalsza część programu (różnica, zapis, potem też GUI) nie zależy już od
wewnętrznej reprezentacji SFML, tylko od naszej struktury.

SFML sam rozpoznaje format pliku, więc oprócz BMP wczytują się też PNG
i JPG (a przy okazji np. TGA). Po wczytaniu każdy z nich ma tę samą postać
RGBA, więc reszta kodu nie musi wiedzieć, skąd obraz przyszedł.

## Zapis obrazu

`saveImage` (i `saveBmp`, które ją woła) robi odwrotnie: z `ImageBuffer`
buduje `sf::Image` i zapisuje go na dysk. Format wybiera SFML po
rozszerzeniu ścieżki: `.bmp`, `.png`, `.jpg`, `.tga`. Przy nieznanym
rozszerzeniu zapis się nie udaje.

Przed zapisem sprawdzamy, czy ścieżka nie jest pusta, czy szerokość
i wysokość są dodatnie oraz czy tablica `rgba` ma wystarczająco dużo
bajtów (`width * height * 4`). Jak coś się nie zgadza albo SFML nie
zapisze pliku, zwracamy `false`.

PNG i BMP zapisują piksele bez strat. JPG jest stratny, po ponownym
wczytaniu kolory mogą się minimalnie różnić, dlatego wyniki porównania
lepiej zapisywać do BMP albo PNG.

Tą samą funkcją zapisujemy zwykły obraz, wynik obrazu różnicowego
i sklejony wycinek.

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

## Wycinek z obu obrazów i sklejenie

Funkcja `cropStitch` dostaje oba obrazy i prostokąt: lewy górny róg `x, y`
oraz szerokość i wysokość `w, h`, w pikselach obrazu. Prostokąt zaznacza
użytkownik, a przeliczenie z pozycji myszy na piksele robi viewport.

Najpierw prostokąt jest przycinany do obszaru, który istnieje w obu
obrazach: x i y nie mogą być ujemne, a prawa i dolna krawędź nie mogą
wyjść poza mniejszy z obrazów. Jak po przycięciu nic nie zostanie
(np. zaznaczenie całkiem poza obrazem albo zerowa szerokość), funkcja
zwraca pusty `ImageBuffer`.

Wynik ma szerokość 2 * cw i wysokość ch, gdzie cw i ch to wymiary
przyciętego prostokąta. Potem idziemy wiersz po wierszu: z lewego obrazu
kopiujemy cw pikseli wiersza do lewej połowy wyniku, a z prawego obrazu
ten sam fragment do prawej połowy. Wiersz to ciągły kawałek pamięci,
więc kopiujemy go w całości (`memcpy`), a nie piksel po pikselu.

Pozycja piksela w tablicy to `(y * width + x) * 4`, osobno dla każdego
obrazu, bo lewy i prawy mogą mieć różną szerokość.

Sklejony wycinek zapisuje się tak samo jak inne wyniki, przez `saveBmp`.
