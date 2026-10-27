# 4. Analiza projektu

## a) Specyfikacja danych wejściowych

Program porównuje dwa obrazy pokazane obok siebie, więc na wejściu potrzebuje
dwóch plików graficznych: obraz lewy i obraz prawy. Użytkownik wybiera je
osobno z dysku (okno otwierania pliku).

Rodzaj danych to bitmapy / zdjęcia rastrowe, czyli siatka pikseli z kolorami.
Postać to zwykły plik na dysku, wskazany ścieżką.

Na wymaganiach podstawowych obsługujemy format BMP. Przy rozszerzeniach
dojdą też PNG i JPG, żeby dało się wczytać typowe zdjęcia bez ręcznej
konwersji.

Od pliku oczekujemy, że da się go poprawnie otworzyć i odczytać kolory R, G, B
(wartości 0-255). Jak w pliku jest kanał alfa, zapamiętujemy go przy wczytaniu,
ale przy liczeniu obrazu różnicowego i tak ustawiamy A na 255.

W wersji podstawowej oba obrazy powinny mieć ten sam rozmiar (szerokość
i wysokość w pikselach). Przy różnych rozmiarach (rozszerzenia) najpierw
wejście w skalę względną - to jest po stronie wyświetlania / viewportu,
nie samego wczytywania pliku.

Jak plik jest uszkodzony, pusty albo w formacie którego nie obsługujemy,
wczytanie kończy się niepowodzeniem (program nie dostaje sensownego obrazu).

Po wczytaniu trzymamy obraz w pamięci w strukturze ImageBuffer: szerokość,
wysokość i tablica pikseli RGBA. Dokładniejszy opis tej struktury będzie
w punkcie o strukturach danych.

## b) Opis oczekiwanych danych wyjściowych

Wyjściem programu (poza tym co widać na ekranie) jest zapis wyniku do pliku.
Format zapisu na start to BMP.

Główny wynik to obraz różnicowy. Dla każdego piksela bierzemy odpowiadające
sobie piksele z lewego i prawego obrazu i liczymy wartość bezwzględną różnicy
składowych: |R1-R2|, |G1-G2|, |B1-B2|. Kanał A ustawiamy na 255.
Tam gdzie obrazy były podobne, wynik jest ciemny; gdzie mocno się różniły -
jaśniejszy. Przy wejściach tego samego rozmiaru wynik ma taki sam rozmiar
jak one.

Drugie wyjście (wymaganie rozszerzone) to wycinek: użytkownik zaznacza
prostokąt na obrazie, program wycina ten obszar z obu stron i skleja obok
siebie (lewy | prawy) do jednego BMP.

Podsumowując: pliki wejściowe to dwa obrazy (BMP, później też PNG/JPG),
pliki wyjściowe to BMP z różnicą albo ze sklejonym wycinkiem. Sam podgląd
na ekranie (powiększenie, przesuwanie) nie jest tu opisywany jako „dane
wyjściowe w pliku”.
