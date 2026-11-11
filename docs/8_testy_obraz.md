# 8. Testowanie (moduł obrazu)

Do sprawdzania ImageCore jest osobny program `test_bmp` (CMake target
o tej samej nazwie). Kompiluje się bez GUI:

```
cd SRC/build
cmake --build . --target test_bmp -j
./test_bmp
```

Ścieżka do sample'ów jest wklejana przy kompilacji (`SAMPLES_DIR`). Jak
wszystko przejdzie, program kończy się kodem 0. Jak któryś test padnie,
pisze na stderr co nie gra i kończy się kodem 1.

## Zestawy danych

W `assets/samples/` leżą pliki do ręcznych i automatycznych testów:

para_same_a.bmp / para_same_b.bmp - dwa BMP 64x64 (ten sam rozmiar).
Używane do round-trip load/save, do diff i do cropStitch.

para_diff_a.bmp (80x60) / para_diff_b.bmp (120x90) - różne rozmiary.
Diff na tej parze ma zwrócić pusty wynik. Przyda się też później przy
skali względnej w viewportcie.

para_same_a.png / para_same_b.jpg - te same obrazki co para_same, tylko
w innych formatach. Sprawdzają, że loadImage nie jest przywiązany do BMP.

Przy testach zapisujących coś na dysk powstają pliki `_tmp_*` (roundtrip,
diff, stitch, format). Są w `.gitignore`, nie idą do repo.

## Testy niezależnych bloków

Round-trip BMP: wczytaj sample, zapisz pod inną nazwą, wczytaj z powrotem
i porównaj piksele. Robione na wszystkich czterech BMP.

diff w pamięci: dwa małe buffery z ręcznie ustawionymi kolorami. Sprawdzane
są dokładne bajty wyniku (|10-15|=5 itd.) oraz to, że diff(a,a) daje czarne
RGB i A=255.

diff na sample'ach: para tego samego rozmiaru, zapis wyniku do BMP
i ponowne wczytanie. Para różnych rozmiarów musi dać pusty buffer.

cropStitch w pamięci: sztuczna „kratka” pikseli (R=x, G=y). Sprawdzane
jest sklejenie lewej/prawej połowy, przycinanie prostokąta wychodzącego
poza obraz, ujemne x/y, różne rozmiary wejść i przypadki z pustym wynikiem.

cropStitch na sample'ach: wycinek 32x32 z pary 64x64, zapis BMP, reload
i porównanie pikseli.

Formaty: zapis/odczyt PNG (piksele mają wyjść takie same) oraz JPG
(sprawdzany tylko rozmiar, bo JPG jest stratny). Zapis z rozszerzeniem
`.xyz` ma się nie udać.

Edge case'y ścieżek: pusta ścieżka, nieistniejący plik, ścieżka z `..`,
pusty buffer przy zapisie, za krótkie `rgba` przy zapisie.

## Testy powiązań i całościowe

Powiązanie load → diff → save i load → cropStitch → save jest w tych
samych testach co wyżej (sample + zapis + reload).

Test całościowy programu (dwa obrazy na ekranie, zoom, pan, zapis różnicy
z menu) jest wspólny z resztą grupy i robi się ręcznie na zbudowanej
aplikacji. ImageCore samo w sobie nie odpala okna.

## Niezmienniki (na tyle ile da się sensownie)

Po udanym loadImage: width > 0, height > 0, rgba.size() == width*height*4.

Po udanym saveImage plik da się z powrotem wczytać (dla BMP/PNG z tymi
samymi pikselami; dla JPG przynajmniej z tym samym rozmiarem).

diff przy tym samym rozmiarze: wynik ma ten sam width/height, A wszędzie 255.

cropStitch przy niepustym wyniku: width == 2*cw, height == ch, cw i ch > 0.
