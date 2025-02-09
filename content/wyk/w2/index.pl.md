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
