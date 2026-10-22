# Plan projektu 17 — Porównywanie obrazów

**Cel:** działający program (C++ / ImGui + SFML z labów) + dokumentacja PDF + archiwum `17.7z` do **1 stycznia 2027**.  
**Bufor bezpieczeństwa:** oddanie gotowe do **20 grudnia** (−8 pkt za każdy dzień spóźnienia z 40).

---

## Wymagania (z `17.pdf` + `zaliczenie/`)

| Warstwa | Co trzeba |
|--------|-----------|
| **Podstawowe (20 pkt)** | 2× BMP obok siebie, pionowa kreska, synchroniczne powiększenie dyskretne (50/100/200/400%…), synchroniczny pan, obraz różnicowy \|RGB₁−RGB₂\|, zapis BMP |
| **Rozszerzone (10 pkt)** | dowolne zoomy, różne rozmiary zdjęć (najpierw skala względna), zaznaczenie obszaru → wycinek z obu → sklejenie → BMP, inne formaty |
| **Oddanie** | C++, bez OpenGL/DirectX; `17.7z` (Ultra) z `DOC/`, `SRC/`, `BIN/`; mail = tylko **link**; docs wg `wzorzec.pdf` / przykładu `Dokumentacja_GFK04.pdf` |

---

## Podział na 3 niezależne moduły

Wspólny kontrakt na start (1 plik nagłówkowy) — potem praca prawie bez kolizji w git.

| Osoba | Moduł | Katalog | Odpowiedzialność | Nie zależy od |
|-------|--------|---------|------------------|---------------|
| **Emilia** | `ImageCore` | `src/image/` | Wczyt/zapis BMP (+ później PNG/JPG), obraz różnicowy, crop+stitch, struktury pikseli | GUI, zoom, pan |
| **Wojtek** | `Viewport` | `src/view/` | Split view + kreska, zoom dyskretny/ciągły, pan sync, skala względna, zaznaczanie ROI | Dialogów plików, zapisu BMP |
| **Krzysztof** | `App` + dostawa | `src/app/`, `docs/`, `packaging/` | Okno ImGui/SFML, menu, dialogi, składanie modułów, CMake, README, `17.7z`, szkielet dokumentacji | Algorytmów obrazu / logiki viewportu |

**Interfejs łączący (ustalany raz, tydzień 0):** np. `ImageBuffer { width, height, rgba[] }` + `loadBmp/saveBmp/diff/cropStitch` oraz `ViewState { zoomL, zoomR, offset, mode }` — Emilia i Wojtek implementują „za” interfejsem; Krzysztof tylko woła API.

---

## Repo GitHub (dzień 1–2)

1. Utworzyć prywatne repo (np. `IUiBG-Projekt-17`), zaprosić Wojtka i Krzysztofa.  
2. Branch: `main` chroniony; praca na `feature/<osoba>-…`, merge przez PR.  
3. Startowa struktura:

```text
/
  README.md
  CMakeLists.txt
  .gitignore
  src/image/   src/view/   src/app/
  assets/samples/   # 2–3 pary BMP do testów
  docs/             # źródło dokumentacji (md/docx → PDF na końcu)
  DOC/ SRC/ BIN/    # docelowa struktura oddania (BIN dopiero przy release)
```

4. Krzysztof: skeleton „puste okno ImGui+SFML kompiluje się”.  
5. Emilia: 1–2 sample BMP (identyczny rozmiar + para różnych rozmiarów).  
6. Zasada: **nie pchać binarek/`.o`/`build/`** — tylko źródła i sample.

---

## Harmonogram (22.10 → 01.01)

### Tydzień 0 — 22–26.10 · Kickoff
**Wspólnie (1 spotkanie ~1,5 h):** kontrakt API, stack, nazwy klas, kryteria „done” dla podstawy.  
| Emilia | Wojtek | Krzysztof |
|--------|--------|-----------|
| Szkic struktur `ImageBuffer`, lista formatów | Szkic `ViewState` + layout split | Repo + CMake + puste okno + `.gitignore` |
| Draft: dane wejściowe/wyjściowe (docs §4a–b) | Draft: UI viewportu (docs §4d fragment) | Szkielet docs wg wzorca + założenia (§1–3) |

### Sprint 1 — 27.10–09.11 · Fundament podstawy
| Emilia | Wojtek | Krzysztof |
|--------|--------|-----------|
| Load/save BMP (własny parser lub SFML `sf::Image` + walidacja) | Split view + pionowa kreska, rysowanie 2 tekstur | Menu: Otwórz lewy/prawy, Zapisz różnicę (stub) |
| Testy jednostkowe load/save na sample | Zoom 50/100/200/400% (zmiana „rozmiaru piksela”) | Podpięcie load → przekazanie bufferów do Viewport |
| | Pan (mysz/scroll) lokalnie | Docs: struktury danych (§4c), wybór narzędzi (§4f) |

**Checkpoint 09.11:** widać 2 BMP obok siebie + zoom dyskretny (nawet bez pełnego sync).

### Sprint 2 — 10–23.11 · Wymagania podstawowe DONE
| Emilia | Wojtek | Krzysztof |
|--------|--------|-----------|
| Algorytm różnicowy \|ΔR\|,|ΔG\|,|ΔB\| | **Synchroniczny** zoom + pan | Pełne podpięcie Zapisz różnicę → BMP |
| Zapis wyniku do BMP | Stabilny clamping offsetów | Docs: zadania (§4e), algorytmy różnicy (§6 część) |
| Edge cases: różne ścieżki, błędy pliku | | Log błędów w UI |

**Checkpoint 23.11 (twardy):** cała **wersja podstawowa** działa end-to-end. To zabezpiecza 20 pkt wykonania.

### Sprint 3 — 24.11–07.12 · Rozszerzenia
| Emilia | Wojtek | Krzysztof |
|--------|--------|-----------|
| Inne formaty (PNG/JPG przez SFML/`stb`) | Dowolny zoom (float) | UX: suwak zoomu, status „różne rozmiary” |
| `crop` + `stitch` → jeden BMP | Skala względna przy różnych rozmiarach | Integracja ROI → wywołanie API Emili |
| | Zaznaczanie prostokąta na jednym panelu | Docs: algorytmy crop/stitch + zoom (§6) |

**Checkpoint 07.12:** rozszerzenia działają; brakuje tylko polish + docs.

### Sprint 4 — 08–20.12 · Docs, testy, oddanie
| Emilia | Wojtek | Krzysztof |
|--------|--------|-----------|
| Docs: kodowanie ImageCore (§7), testy obrazu (§8) | Docs: kodowanie Viewport (§7), testy UI (§8) | Złożenie PDF, spójność, rysunki ekranu |
| Zestawy testowe (para różna, ROI, formaty) | Scenariusze ręcznego testu sync | `BIN/` (build Release), `SRC/`, `DOC/` → `17.7z` Ultra |
| | | README + link do archiwum (Drive/GH Release) |

**Soft deadline: 20.12** — archiwum + mail z linkiem gotowe.  
**21–31.12:** tylko poprawki krytyczne / retest na czystej maszynie.  
**01.01:** absolutny hard stop.

---

## Dokumentacja (25 pkt) — mapowanie na osoby

Zgodnie z `arkusz_oceny.pdf` / `wzorzec.pdf`:

| Sekcja | Punkty | Kto prowadzi |
|--------|--------|--------------|
| Założenia wstępne | 1 | Krzysztof (+ akceptacja grupy) |
| Dane wejściowe / wyjściowe | 1+1 | Emilia |
| Struktury danych | 2 | Emilia (obraz) + Wojtek (view state) |
| Specyfikacja UI | 1 | Wojtek + Krzysztof |
| Wyodrębnienie zadań | 2 | Krzysztof (tabela modułów) |
| Wybór narzędzi | 1 | Krzysztof |
| Podział pracy i czas | 1 | wszyscy (ten plan → do docs) |
| Algorytmy | 5 | Emilia (diff/crop/I/O) + Wojtek (zoom/pan/skala) |
| Kodowanie | 6 | każdy swój moduł (~2 pkt „wartości”) |
| Testowanie | 3 | każdy swoje testy + 1 wspólny scenariusz E2E |
| Wdrożenie / wnioski | 1 | Krzysztof |

Pisać **równolegle od tygodnia 0**, nie „na końcu” — to 25% oceny.

---

## Synchronizacja (minimalna, żeby zachować niezależność)

- **Spotkanie:** 30–45 min co niedzielę (status + konflikty API).  
- **PR review:** autor innego modułu (Emilia↔Wojtek, Krzysztof na CMake/docs).  
- **Zakaz:** edycji cudzych katalogów bez PR z tagiem `@owner`.  
- **Integracja:** Krzysztof merge’uje do `main` tylko gdy build zielony.

---

## Kryteria „projekt skończony”

1. Spełnione wszystkie wymagania podstawowe + (docelowo) rozszerzone.  
2. Dokumentacja PDF kompletna wg wzorca.  
3. `17.7z` z `DOC/`, `SRC/`, `BIN/` (Ultra), program startuje z `BIN/` na czystym systemie grupy.  
4. Mail z **linkiem** (nie załącznikiem) wysłany najpóźniej 20.12 (rezerwa do 01.01).

---

## Szybki start (dziś / jutro)

1. Krzysztof tworzy repo i wrzuca skeleton.  
2. Emilia + Wojtek akceptują 1-stronicowy kontrakt API w `docs/api.md`.  
3. Każdy zakłada branch i bierze pierwszy task ze Sprintu 0/1.
