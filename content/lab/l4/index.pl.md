---
title: "L4"
date: 2022-02-05T17:26:02+01:00
weight: 40
---

# Laboratorium 4
## Narzędzia

W tym laboratorium zajmiemy się poznaniem nowego narzędzia do budowania programów: **CMake**.
Umożliwi to nam używanie debuggera **gdb** oraz skorzystanie z biblioteki do testów oprogramowania **GTest**.

### Kodowanie base32hex
Tematyka laboratorium obejmuje kodowanie strumieni bajtów.
Jest to zadanie zapisania arbitralnego strumienia bajtów w formacie wcześniej ustalonym z odbiorcą danych.
W przypadku dzisiejszych laboratorium użyjemy kodowania [**base32hex**](https://en.wikipedia.org/wiki/Base32#Base_32_Encoding_with_Extended_Hex_Alphabet_per_%C2%A77), które zapisuje bajty wykorzystując 32 znaki `0-9A-V`.
Jest to zapis, który powoduje zwiększenie objętości zapisanych danych.
W zamian uzyskujemy możliwość zapisu danych w miejscach, gdzie powinien znaleźć się tylko tekst (np. nazwy plików lub pismo wydane drukiem).

Aby skorzystać z tego sposobu zapisu, potrzebne są dwa procesy: **kodowania** i **rozkodowania**.

**Implementacja tych procesów jest poza zakresem tego laboratorium i zajęłaby zbyt dużo czasu.**

Z tego powodu otrzymasz pewną implementację tych procesów w postaci dwóch klas: **Encoder** oraz **Decoder**.

#### Encoder base32hex

Klasa `Encoder` implementuje proces kodowania strumienia bajtów do `std::string`.
Posiada ona dwie metody:
* `pushBytes`, która przyjmuje wskaźnik na początek strumienia oraz długość przekaznego strumienia.
* `encodedString`, która zwraca `std::string` zawierający zakodowany strumień bajtów.

Typowe użycie to:
1. Stworzenie obiektu klasy `Encoder`.
2. Zawołanie raz (lub więcej) funkcji `pushBytes`.
3. Uzyskanie zakodowanego napisu przy pomocy wywołania `encodedString`.

#### Decoder base32hex

Klasa `Decoder` implementuje proces dekodowania napisu w postaci `std::string` do strumienia bajtów.
W czasie konstrukcji obiekt musi poznać cały napis z zakodowaną wiadomością.
Posiada ona dwie metody:
* `isEmpty`, która informuje czy można zdekodować strumień bajtów z napisu.
* `pullBytestream`, która zwraca `std::vector<std::byte>` zawierający rozkodowany strumień bajtów.

Typowe użycie to:
1. Stworzenie obiektu klasy `Decoder`. W czasie konstrukcji należy przekazać zakodowany napis.
2. Zawołanie funkcji `isEmpty`, aby sprawdzić czy dekoder może rozkodować napis. Jeśli tak, to wykonaj pkt. 3. W przeciwnym wypadku koniec.
3. Uzyskanie zdekodowanego strumienia przy pomocy wywołania `pullBytestream`.
4. Idź do pkt.2

### Korzystanie z obcego kodu

Gdy w naszym projekcie korzystamy z kodu napisanego przez innego programistę, ponosimy ryzyko, że obcy kod posiada błędy.
Z tego powodu należy korzystać z bibliotek renomowanych, które są wysokiej jakości.

Jeśli jednak musimy skorzystać z biblioteki wątpliwej jakości (jak w przypadku tych laboratoriów), to kluczowe są dwie zasady:
1. Mimo podejrzenia istnienia błędów należy zaufać, że kod działa prawidłowo.
2. Jeśli jednak w czasie korzystania z biblioteki znajdziemy błąd to mamy dwie drogi:
    * Jako autor projektu korzystającego z obcego kodu bierzemy odpowiedzialność za całokształt i naprawiamy błąd (*trudna ścieżka*).
    * Kontaktujemy się z autorem biblioteki i prosimy go o naprawienie błędu (*trochę łatwiejsza ścieżka*).

Powyższa praktyka ma charakter **reaktywny** i sprawdza się *w większości przypadków*.
Proszę pamiętać, że błąd w naszym projekcie może być oczekiwaną własnością w innym projekcie.

### Projekt CMake

W tym etapie zadania chcemy skorzystać z biblioteki `base32`.
Twórca biblioteki poza dwoma klasami dostarcza plik Makefile opisujący zbudowanie przykładowego programu.
Zawiera on dodatkowo plik `main.cpp`, który symuluje program `basenc` z parametrem `--base32hex`.
Kod źródłowy biblioteki możesz pobrać za pomocą polecenia
```bash
wget -w 1 -A "*.cpp" -A "*.hpp" -A "makefile" --ignore-case -nH -r -np --cut-dirs=2 https://cpp.mini.pw.edu.pl/lab/l4/src/
```

**Proszę potraktować otrzymany kod w pliku main.cpp jako czarną skrzynkę. Wewnątrz znajdują się funkcje wychodzące poza zakres tego przedmiotu i nie będziemy ich omawiać.**

Twoim zadaniem jest stworzenie projektu `CMake` na podstawie logiki z pliki `Makefile`.
Aby tego dokonać stwórz następującą strukturę katalogów:
```text
 - src/
 |  - main.cpp
 |  - CMakeLists.txt
 - lib/
 |  - base32/
 |  |  - base32_def.hpp
 |  |  - CMakeLists.txt
 |  |  - decoder.cpp
 |  |  - decoder.hpp
 |  |  - encoder.cpp
 |  |  - encoder.hpp
 |  - CMakeLists.txt
 - tests/ # No cpp files for now
 |  - CMakeLists.txt
 - CMakeLists.txt
```

W pliku `lib/base32/CMakeLists.txt` należy zdefiniować target `base32`, który reprezentuje budowanie **biblioteki statycznej** zawierającej kod biblioteki.
Dwa pliki: `encoder.cpp` oraz `decoder.cpp` należy załączyć jako źródła targetu `base32`.
Dołączenie plików nagłówkowych nie jest konieczne.
CMake potrafi samodzielnie ustalić, które pliki źródłowe należy przebudować po zmianie pliku nagłówkowego.

W pliku `src/CMakeLists.txt` należy zdefiniować target reprezentujący budowanie **pliku wykonywalnego** `l4_base32`.
Powinien polegać prywatnie na targecie `base32`.

W pliku `lib/CMakeLists.txt` użyj polecenie `add_subdirectory` aby załączyć do projektu folder `tests/base32`.
Podobnie należy postąpić w głownym pliku `CMakeLists.txt` włączając do projektu trzy foldery `src`, `lib` oraz `tests`.
Jest to też odpowiednie miejsce na ustawienie flag kompilacji wspólnych dla wszystkich targetów w projekcie.

Aby sprawdzić, czy skompilowany program działa prawidłowo, wykonaj następujące kroki:
1. Skonfiguruj projekt CMake i wykonaj budowanie targetu `l4_base32`.
2. Wykonaj polecenie `echo "abba" | path/to/built/executable/l4_base32`.
3. Wykonaj polecenie `echo "abba" | basenc --base32hex`.
4. Wyjścia kroków 2. oraz 3. powinny być identyczne (konkretnie `C5H64O8A`).

### Uruchomienie debuggera

W świecie GNU Linux standardem stał się debugger `gdb` (GNU debugger).
Debugger pozwala nam śledzić i modyfikować proces wykonania dowolnego programu.
W przypadku dzisiejszego laboratorium będziemy używać go do znalezienia błędu w bibliotece `base32`.
Przed tym należy przygotować swój program oraz środowisko programistyczne do używania debuggera.

Pierwszym krokiem jest upewnienie się, że każda jednostka translacji (plik `.o`) składający się na nasz program został zbudowany z flagą `-g`.
Flaga ta dodaje do pliku wykonywalnego niezbędne wskazówki, które pozwalają odnaleźć linijkę w kodzie źródłowym związaną z aktualnie wykonywaną instrukcją procesora.
Dodatkowo da nam także możliwość połączenia zawartości pamięci ze zmiennymi zdefiniowanymi w kodzie źródłowym.

W przypadku projektów CMake istnieją domyślnie zdefiniowane konfiguracje `Debug` oraz `Release`.
Skojarzone są z nimi domyślne zestawy flag kompilatora (w przypadku `Debug` jest tam flaga `-g`).
Aby wybrać, w jakiej konfiguracji będą wykonywana kompilacja należy przekazać flagę `CMAKE_BUILD_TYPE` w trakcie konfiguracji projektu (np. `-DCMAKE_BUILD_TYPE=Debug`).
Jeśli potrzebujemy flag innych niż przygotowane przez CMake, należy zdefiniować zmienną `CMAKE_CXX_FLAGS`.
Rozszerzy to używane flagi w trakcie budowania.
Jest to dobry sposób na włączenie address sanitizera.

Aby przekonać się, jakie flagi używane są w trakcie budowania, podaj przy konfigurowaniu CMake flagę `-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON`.

#### Tryb konsolowy

Zacznijmy od najprostszego sposobu użycia `gdb` - trybu konsolowego.
Można go użyć, nie posiadając skonfigurowanego środowiska programistycznego.
Aby uruchomić nasz program pod kontrolą debuggera, należy wydać polecenie `gdb path/to/built/executable/l4_base32`.

W tym momencie debugger powinien załadować wszystkie symbole związane z kodem wykonywalnym.
Gdy wszystko będzie gotowe, debugger będzie oczekiwać na polecenie `run`, które uruchomi program.
Warto przed uruchomieniem programu stworzyć potrzebne breakpointy lub wykonać różne akcje instrumentujące.

Ten tryb działania debuggera jest bardzo potężny i elastyczny, jednak nie należy do prostych w użyciu.
Aby odkryć pewną funkcję debuggera, należy się o niej dowiedzieć z [dokumentacji użytkownika](https://sourceware.org/gdb/current/onlinedocs/gdb.html/) i wprowadzić odpowiednie polecenie.
Nie jest to najszybszy sposób na zapoznanie się z podstawowymi funkcjonalnościami tego narzędzia.

Jeśli jednak jest to nasze jedyne wyjście (np. trzeba wykonać debuggowanie na komputerze bez środowiska graficznego), to istnieją w internecie tzw. *cheat-sheety* przygotowane przez bardziej doświadczonych użytkowników.
Ułatwiają one znalezienie potrzebnej akcji do wykonania w trakcie użytkowania (przykładowy [cheat-sheet](https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf)).

**Proszę pamiętać, że cheat-sheet nie zastąpi [dokumentacji użytkownika](https://sourceware.org/gdb/current/onlinedocs/gdb.html/) oraz polecenia `help`.**

#### Tryb graficzny

Używanie `gdb` w środowisku graficznym pozwala nam uprościć użytkowanie debuggera.
Mamy do dyspozycji integrację z naszym środowiskiem graficznym albo niezależne programy okienkowe.
Na laboratorium zachęcamy do korzystania z integracji ze swoim IDE, ponieważ projekt CMake umożliwia takie podejście.
Jeśli jednak nasze IDE nie wspiera integracji lub nie skonfigurowaliśmy go na odpowiedniem poziomie, można posłużyć się bardzo dobrym projektem [`gdbgui`](https://github.com/cs01/gdbgui/), który jest niezależnym webowym interfejsem graficznym.

Kiedy używamy trybu graficznego, należy skorzystać z instrukcji do używanego środowiska.

#### Zatrzymanie programu w funkcji `main`

Aby przećwiczyć użycie `gdb`, twoim zadaniem jest zatrzymać się na wejściu do funkcji `main`.
Wykonaj ćwiczenie w dwóch trybach: konsolowym i graficznym.

W przypadku trybu konsolowego wystarczy wydać polecenie `b main` oraz `run`.
Pierwsze polecenie stworzy breakpoint na wejściu do funkcji, a drugi rozpocznie działanie programu.
Dla trybu graficznego tworzenie breakpointów odbywa się zazwyczaj poprzez naciśniecie lewym przyciskiem myszy na numer linii, w której chcemy zatrzymać nasz program.
Jako symbol breakpoint'a pojawi się przy tej linii czerwona kropka.
Szczegóły używania zależą od wybranego środowiska programistycznego.

### Użycie biblioteki GTest oraz przygotowanie testów jednostkowych

Kiedy mamy skonfigurowany projekt wraz z debuggerem, możemy przejść do odkrywania błędów w dostarczonym kodzie.
W przypadku tych laboratoriów napiszemy testy jednostkowe dla dostarczonych klas `Encoder` oraz `Decoder`.
W tym celu dołączymy kolejną bibliotekę do naszego projektu: `GTest`.

#### Integracja GTest z CMake

Idąc za przykładem z [dokumnetcji GTesta](https://google.github.io/googletest/quickstart-cmake.html) wykorzystamy mechanizm `FetchContent` do pobrania kodu źródłowego biblioteki.
W tym celu należy edytować plik `tests/CMakeLists.txt` i dodać do niego następujące linijki:

```cmake
# Download a cmake library during the configure phase
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.16.0
)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)
```

Od tego momentu mamy do dyspozycji dwa targety: `GTest::gtest` oraz `GTest::gtest_main`.
Obydwa zawierają wszystkie funkcjonalności biblioteki, ale drugi zawiera przy okazji definicję funkcji `main` dla programu testującego.

Aby przygotować program testujący dwie dostarczone funkcje, przygotuj dwa nowe pliki: `tests/encoder.cpp` oraz `tests/decoder.cpp`.
Program testujący jest zwykłym plikiem wykonywalnym.
Przygotuj nową definicję takiego pliku o nazwie `base32_test`, który zawiera dwa pliki `.cpp` znajdujące się w folderze `tests`.

Aby skorzystać z klas `Encoder` oraz `Decoder` w testach, target `base32_test` powinien posiadać prywatną zależnośc do `base32`.
Drugą prywatną zależnością programu powinien zostać `GTest::gtest_main`, który zapewni wszystkie funkcjonalności dostarczone przez bibliotekę GoogleTest.

Ostatnim elementem jest zawołanie funkcji `gtest_discover_tests(base32_test)` na końcu pliku `tests/CMakeLists.txt`.
Dzięki niej CMake jest świadomy, jakie funkcje znajdują się we wnętrzu pliku wykonywalnego.
Umożliwia to lepszą integrację z niektórymi środowiskami programistycznymi.


#### Testy dla klasy Decoder

Scenariusz testowy klasy `Decoder` polega na wczytaniu przygotowanego ręcznie zakodowanego tekstu, zdekodowanie go oraz porównanie z oczekiwanym ciągiem bajtów (w tym przypadku napisem).
Dla każdego przypadku należy stworzyć instancje klasy `Decoder` w zależności od otrzymanego zakodowanego tekstu.
Zaimplementuj podane poniżej przypadki:

* `"" -> ""`
* `"CO======" -> "f"`
* `"CPNG====" -> "fo"`
* `"CPNMU===" -> "foo"`
* `"CPNMUOG=" -> "foob"`
* `"CPNMUOJ1" -> "fooba"`
* `"CPNMUOJ1E8======" -> "foobar"`
* `"CPNMUOJ1E8======;CPNMU===" -> "foobar" and "foo"` - *zawołaj dwukrotnie funkcję `pullBytestream`*

Jeżeli zastanawiasz się jak wykonać konwersję `std::string` do `std::vector<std::byte>` to tutaj znajdziesz podpowiedź jak powinien wyglądać typowy test:

```cpp
char input[] = "12235343465ABDF====";
char expected_chars[] = "expected_decoded_text";

// Copy string byte-by-byte to the vector
std::vector expected(
    reinterpret_cast<std::byte *>(expected_chars),
    reinterpret_cast<std::byte *>(expected_chars) + sizeof(expected_chars) - 1
    );

std::vector<std::byte> res;
// Use a decoder and fill the res vector here...

ASSERT_EQ(res.size(), expected.size());
for (int i = 0; i < res.size(); ++i)
    EXPECT_EQ(res[i], expected[i]) << "Different value at " << i;
```

#### Testy dla klasy Encoder

Scenariusz testowy klasy `Encoder` polega na przygotowaniu napisu, wykonaniu kodowania i porównaniu go ze spodziewanym wynikiem.
Dla każdego przypadku istnieją wspólne zmienne.
Przygotuj *fixturę* zawierającą pole `Encoder encoder` z akcesorem `protected`.
Wykorzystaj ją do implementacji poniższych przypadków:
* `"" -> ""`
* `"f" -> "CO======"`
* `"fo" -> "CPNG===="`
* `"foo" -> "CPNMU==="`
* `"foob" -> "CPNMUOG="`
* `"fooba" -> "CPNMUOJ1"`
* `"foobar" -> "CPNMUOJ1E8======"`
* `"foobar" and "foo" -> "CPNMUOJ1E8======;CPNMU==="` - *zawołaj funkcje `pushBytes` dwa razy*

### Wczytanie *core dump* z momentu wystąpienia błędu

W pliku [weird_bug.core](weird_bug.core) znajdziesz *coredump* z wykonania programu, który zakończył niespodziewanie.
Wczytaj go do debuggera i spróbuj zrozumieć, dlaczego nastąpił crash.

Jako dodatkowe ćwiczenie możesz spróbować zgadnąć jakie wejście spowodowało problem i reprodukować błąd.
Dobrą praktyką jest przygotowanie testu, który eksponuje ten problem.
Znacznie ułatwia to namierzenie przyczyny i naprawę błędu.
Dokładne szczegóły działania biblioteki są poza zakresem laboratorium, więc naprawa błędu jest zadaniem dodatkowym.

### Rozwiązanie laboratoriów
Kod z przygotowaną struktura projektu w CMake, napisanymi testami i naprawionymi błędami można pobrać za pomocą polecenia
```bash
wget -w 1 -A "*.cpp" -A "*.hpp" -A "cmakelists.txt" --ignore-case -nH -r -np --cut-dirs=2 https://cpp.mini.pw.edu.pl/lab/l4/solution/
```
