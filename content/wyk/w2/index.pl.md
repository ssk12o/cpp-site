---
title: "W2 - Pamięć"
date: 2022-02-05T18:00:18+01:00
weight: 20
---

# Wykład 2 - Pamięć

Zakres:

* obiekty
* system typów
  * typy podstawowe
  * tablice
  * enumeracje
  * referencje
  * wskaźniki
  * struktury, klasy i unie
* rozmiar i wyrównanie
* własności const/volatile
* trwałość obiektów
* czas życia obiektów
* operatory new/delete
* address sanitizer (ASAN)

### Obiekty

Programy w C++ manipulują [obiektami](https://en.cppreference.com/w/cpp/language/object).
Cechy obiektów:
* rozmiar (`sizeof()`)
* wyrównanie (`alignof()`)
* trwałość pamięci (automatyczna, statyczna, dynamiczna, thread-local)
    * określa zasady, kiedy pamięć na obiekt będzie pozyskiwana i zwalniana
* czas życia
    * jasno określone punkty początku i końca życia obiektu w czasie wykonania programu
* typ
* wartość
* nazwa (opcjonalna)

Zadeklarowane obiekty (i referencje) nazywamy **zmiennymi**.

Przykłady obiektów:
```cpp
long global;
 
int uninit;
int x = 3;
vector<int> v;
int* ptr = &x;

int* ptr = new int(3);
// ...
delete ptr;
```

### Rozmiar, adres, wyrównanie

Obiekty żyją **w pamięci** programu. Pamięć składa się z bajtów, posiadających adresy.

Każdy obiekt ma **rozmiar** pozyskiwany operatorem `sizeof()`. To liczba zajmowanych przez obiekt bajtów.
Obiekty (z pewnymi wyjątkami) muszą zajmować przynajmniej 1 bajt, tak by miały unikalne adresy.
Ciąg bajtów w pamięci, które zajmuje obiekt, rozmiaru `sizeof(val)`, jest nazywany **reprezentacją obiektu**.  

Typ obiektu narzuca minimalne wymagania na **wyrównanie** pozyskiwane operatorem `alignof()`.
To liczba, zawsze potęga dwójki, przez którą adres musi być podzielny, lub inaczej,
jest to odstęp, który musi być zachowany pomiędzy następującymi po sobie obiektami tego samego typu.
Można zażądać większego, bardziej restrykcyjnego wyrównania
za pomocą operatora [alignas](https://en.cppreference.com/w/cpp/language/alignas).

Każdy obiekt ma **adres** pozyskiwany operatorem `&`.
Przez adres obiektu należy rozumieć adres pierwszego zajmowanego bajtu.

![memory.svg](memory.svg)


```cpp
struct Point
{
    int x;
    int y;
};

int main()
{
    char c = 'a';
    int x = 3;
    std::vector<int> vec = {1, 2, 3};

    std::cout << "sizeof(c) = " << sizeof(c) << ", alignof(char) = " << alignof(char) << ", &c = " << static_cast<void*>(&c) << '\n';
    std::cout << "sizeof(x) = " << sizeof(x) << ", alignof(int) = " << alignof(int) << ", &x = " << &x << '\n';
    std::cout << "sizeof(vec) = " << sizeof(x) << ", alignof(vector<int>) = " << alignof(int) << ", &vec = " << &vec <<'\n';

    Point p = {1, 2};

    std::cout << "sizeof(p) = " << sizeof(p) << ", alignof(Point) = " << alignof(Point) << ", &p = " << &p <<'\n';
    std::cout << "sizeof(p.x) = " << sizeof(p.x) << ", alignof(int) = " << alignof(int) << ", &p.x = " << &p.x <<'\n';
    std::cout << "sizeof(p.y) = " << sizeof(p.y) << ", alignof(int) = " << alignof(int) << ", &p.y = " << &p.y <<'\n';

    return 0;
}
```
Source: [addresses.cpp](addresses.cpp)

Obiekty mogą zawierać **podobiekty**, np.:
* podobiektami struktur są pola struktury
* podobiektami tablic są ich elementy

Podobiekty zajmują tę samą przestrzeń co nadobiekt, mogą więc współdzielić z nim adres.
Jeżeli 2 żyjące obiekty nie są zagnieżdżone, to muszą zajmować
rozłączne obszary pamięci i mieć unikalne adresy. Porównanie adresów
jest wtedy właściwą metodą sprawdzenia _czy to ten sam obiekt_.

### Typy postawowe

C++ zawiera szereg wbudowanych [typów podstawowych](https://en.cppreference.com/w/cpp/language/types).

#### void

Typ pusty. Nie ma obiektów typu `void`. Używany jako typ zwracany w procedurach i 
do konstrukcji wskaźników `void*`.

#### bool

Typ mogący przechowywać jedną z dwóch wartości `true` lub `false`.
Wyrażenia logiczne są typu `bool`.
C++ **nie specyfikuje** jak wygląda reprezentacja tego typu w pamięci.

```cpp
int x = 0;
bool flag = x > 3;
if (flag) {
  // ...
  flag = false;
}
```

#### Typy całkowitoliczbowe



#### Typy znakowe

#### Typy zmiennoprzecinkowe

#### std::nullptr_t

Typ pustego wskaźnika `nullptr`. Wprowadzony dla lepszej kontroli typów.
```cpp
void* p = nullptr;
char* cp = nullptr;
```
Znane z C macro `NULL` było równe `0`, czyli było liczbą. Prowadzi to do niejednoznaczności, np.

```cpp
void func(int);
void func(void*);
func(NULL); // ambiguous
```

Wartości typu `nullptr_t` są konwertowalne na dowolny inny typ wskaźnikowy.

#### 

### Typy inne

### Typowe błędy

Bezpośredni dostęp do pamięci daje ogromne możliwości, ale i otwiera drogę do licznych, trudnych do wychwycenia okiem błędów:
* użycie obiektu ze stosu po jego zwolnieniu
```cpp
int* foo() {
     int x = 0;
     return &x;
}

int main() {
    int* ptr = foo();
    *ptr = 1;
    return 0;
}
```
Source: [stack-use-after-free.cpp](asan/stack-use-after-free.cpp)
* użycie obiektu po zwolnieniu za pomocą `delete`
```cpp
int main() {
    int* ptr = new int{3};
    *ptr = 0;
    delete ptr;
    *ptr = 1;
    return 0;
}
```
Source: [heap-use-after-free.cpp](asan/heap-use-after-free.cpp)
* podwójne zwolnienie za pomocą `delete`
```cpp
void add(int* acc, int* ptr) {
       *ptr = 0;
       delete ptr;
}

int main() {
    int acc = 0;
    int* ptr = new int{3};
    add(&acc, ptr);
    add(&acc, ptr);
    return 0;
}
```
Source: [double-free.cpp](asan/double-free.cpp)
* zwolnienie za pomocą niepoprawnego adresu
```cpp
int acc = 0;

void add(int* ptr) {
       acc += *ptr;
       delete ptr;
}

int main() {
    int a = 0;
    int* b = new int{3};
    add(&a);
    add(b);
    return 0;
}
```
Source: [invalid-free.cpp](asan/invalid-free.cpp)
* niezwolnienie pamięci w czasie działania programu
```cpp
#include <cstddef>

int main() {
    const std::size_t size = 128;
    auto arr = new int*[size];
    for (int i = 0; i < size; i++) {
        arr[i] = new int{i};
    }
    int sum = 0;
    for (std::size_t i = 0; i < size; i++) {
        sum += *arr[i];
    }
    delete[] arr;
    return 0;
}

```
Source: [memory-leak.cpp](asan/memory-leak.cpp)
* przepełnienie bufora na stosie
```cpp
#include <cstring>

int main() {
    char command[10];
    char output[250];
    std::strcpy(command, "/usr/bin/verylongcommand");
    std::strcpy(output, command);
    return 0;
}
```
Source: [stack-buffer-overflow.cpp](asan/stack-buffer-overflow.cpp)
* przepełnienie bufora na stercie
```cpp
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i <= vec.size(); i++) {
        vec[i] = 2 * vec[i];
    }
    return 0;
}
```
Source: [heap-buffer-overflow.cpp](asan/heap-buffer-overflow.cpp)
* przepełnienie statycznego (globalnego) bufora

Wszystkie powyższe programy są niepoprawne, ale najgorsze błędy to takie, które nie
ujawniają się natychmiastowo. Program (błędny) może tak funkcjonować miesiącami
w produkcyjnym środowisku.

### Address Sanitizer

Z pomocą przychodzą nowoczesne narzędzia. Postawowym, bez którego ciężko obecnie
wyobrazić sobie pracę nad większym projektem w C++ jest Address Sanitizer (ASan)
wprowadzony przez [Google](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/37752.pdf).

ASan instrumentuje produkowany kod maszynowy, dodając do niego instrukcje sprawdzające
poprawność odwołań do pamięci i przerywając działanie programu natychmiast po wykryciu błędu.
Jest częścią kompilatora, którą trzeba jawnie włączyć, typowo przez podanie flagi takiej jak
`-fsanitize=address` w lini zlecenie. Narzędzie zwiększa użycie pamięci i procesora
przechowując dodatkowe dane i dodając instrukcje, dlatego zwykle jest używane tylko
w konfiguracjach deweloperskich.

```shell
g++ -g -fsanitize=address asan/stack-buffer-overflow.cpp -o prog.exe && ./prog.exe
```

Przykładowe wyjście z błędem prezentuje miejsce w którym nastąpiło niepoprawne odwołanie do pamięci:

```
==9118==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7c3308f0902a at pc 0x7c330b6fb303 bp 0x7fffad4a1a20 sp 0x7fffad4a11c8
WRITE of size 25 at 0x7c3308f0902a thread T0
    #0 0x7c330b6fb302 in memcpy ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors_memintrinsics.inc:115
    #1 0x5769492672f4 in main asan/stack-buffer-overflow.cpp:6
    #2 0x7c330ae2a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7c330ae2a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x576949267164 in _start (/home/saqq/repos/cpp-site/content/wyk/w2/prog.exe+0x1164) (BuildId: 69d828a34f8704fac3eb3dc2ecfaaa8eaf256b24)
```

#### Jak to działa

ASan przechowuje w dodatkowym segmencie pamięci dodatkowe dane 
opisujące stan pamięci właściwej - sterty, stosu, itd. To tzw. _shadow memory_. 
Na każde 8 bajtów przypada jeden _shadow byte_ opisujący stan tych 8'miu bajtów, którego
wartość mówi czy ten region jest poprawny czy nie:
* 0 oznacza, że wszystkie 8 bajtów jest poprawne
* wartość `n` z zakresu `1-7` oznacza że pierwszych `n` bajtów jest poprawnych
* wartość ujemna oznacza, że cały blok jest niepoprawny (_poisoned_)

Mapowanie adresu na adres bajtu shadow jest prostą operacją:
```
ShadowAddress = (RealAddress >> 3) + ShadowOffset
```

Kompilator z włączonym ASan'em dodaje sprawdzenie wartości bajtu shadow przy każdym odwołaniu 
do pamięci.

W celu wykrywania przepełnień buforów, nawet przyległych do siebie, ASan dodaje 
mały niepoprawny region przed i po każdej alokacji na stosie i stercie (tzw. _redzone_).
Robi to modyfikując instrukcje alokujące ramkę stosu każdej funkcji, oraz podmieniając
funkcje alokujące `malloc/realloc/free`.

```mermaid
block-beta
    r1["Redzone"] int r2["Redzone"] char["char c[10]"] r3["Redzone"] double r4["Redzone"]
    style r1 fill:#EF5350,stroke-width:0
    style r2 fill:#EF5350,stroke-width:0
    style r3 fill:#EF5350,stroke-width:0
    style r4 fill:#EF5350,stroke-width:0
```
