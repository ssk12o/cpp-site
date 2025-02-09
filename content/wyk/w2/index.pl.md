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

