---
title: "W5 - STL"
date: 2022-02-05T18:00:18+01:00
weight: 50
---

# Wykład 5 - STL

Zakres:

* Przeładowywanie operatorów
* Konwersje
* Klasy zagnieżdżone i lokalne
* Iteratory
* `auto`
* _ranged-for-loop_
* Standard Template Library
* Funkcje lambda
* Algorytmy STL
* Ranges

## Przeładowywanie operatorów

W C++ można implementować zachowanie operatorów dla własnych typów innych niż wbudowane.
Ten zabieg potrafi znacznie uprościć korzystanie z obiektów naszej klasy.

```cpp
std::string text = "Hello";
text.append(", world!");
text += ", world";
````

Poznaliśmy już ten mechanizm przy okazji operatorów przypisania i przeniesienia `T& operator=(const T&)` i
`T& operator=(T&&)`. Własne operatory to tak naprawdę metody o specjalnej nazwie: `operator@`, gdzie `@`
jest konkretnym operatorem, takim jak `+=`. Typ biblioteczny `std::string` dostarcza 
[operator +=](https://en.cppreference.com/w/cpp/string/basic_string/operator%2B%3D implementujący naturalnie oczekiwaną
konkatenację ciągów znakowych:

```cpp
string& operator+=( const string& str );
```

Bardziej złożony przykład klasy `vector3d` implementującej operacje arytmetyczne na wektorach jako operatory:

```cpp
class vector3d {
    float _x, _y, _z;
public:
    /* ... */
    
    vector3d& operator+=(const vector3d& other) {
        _x += other._x;
        _y += other._y;
        _z += other._z;
        return *this;
    }

    vector3d operator+(const vector3d& other) const {
        vector3d result = *this;
        result += other;
        return result;
    }

    /* ... */
};
```

Operatory arytmetyczne modyfikujące lewą stronę t.j. `+=`, `*=` zwykle zwracają referencję.
Operatory produkujące wartości tymczasowe takie jak `+`, `*` zwykle zwracają przez wartość.

Operatory można implementować jako funkcje składowe klasy lub jako funkcje swobodne, np.:

```cpp
vector3d operator*(float value, const vector3d& other) {
    vector3d result = other;
    result *= value;
    return result;
}
```

Typowym przykładem jest przeładowanie operatora `<<` dla strumieni wyjściowych:

```cpp
std::ostream& operator<<(std::ostream& os, const vector3d& v) {
    os << "vector3d(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
/* ... */
std::cout << vector3d(1, 2, 3) << std::endl;
```

W C++ można przeładowywać [mnóstwo operatorów](https://en.cppreference.com/w/cpp/language/operators).
W rzeczywistości jedynymi, których nie można przeładowywać to operator zakresowy `::`, operatory wyłuskania `.`, `.*` i
operator trójargumentowy `?:`. Można natomiast przeładować:
* operatory arytmetyczne (`+`, `*`, `%`, `++`, `--`, `-`, ...),
* operatory logiczne (`&&`, `||`, ...),
* operatory porównania (`<`, `>`, `==`, ...), 
* operatory bitowe (`&`, `|`, `^`, ...),
* operatory przesunięć (`<<`, `>>=`, ...),
* operatory wyłuskania `->` i `->*`,
* operator `,`,
* operatory wywołania `()` i indeksowania `[]`,
* operatory `new`, `new[]`, `delete`, `delete[]`
* operator `co_await`

Język tłumaczy użycie operatora na wywołanie odpowiedniej metody, np.:
```cpp
T a, b;
a + b;    // (a).operator+(b)
-a        // (a).operator-()
a[b...]   // (a).operator[](b...)
a->x      // ((a).operator->())->x
++a       // (a).operator++()
a++       // (a).operator++(0)
```

> Jeżeli operator jest przeładowany jako funkcja swobodna, to teoretyczna składnia była inna, np.: `operator+(a, b)`.

Przeładowywanie typowych, dwuargumentowych operatorów jest analogiczne.
Rozważmy ciekawsze przypadki.

### Operatory unarne

Wyrażenia `+a` i `-a` wykorzystują operatory _unary plus_ i _unary minus_.
Implementowane jako składowe klasy nie mają argumentów:

```cpp
class Int {
  int value;
public:
    /* ... */
    Int operator+() { return *this; }
    Int operator-() { return Int(-value); }
    /* ... */
};
```
Source: [overloading_arithmetic.cpp](overloading_arithmetic.cpp)

### Pre- i post- inkrementacje

Jak rozróżnić między wyrażeniami `++a` i `a++`?
Post inkrementacje/dekrementacje posiadają dodatkowy parametr
typu `int`, którego pre- operatory nie posiadają:

```cpp
Int& operator++() {
    ++value;
    return *this;
}

Int operator++(int) {
    Int tmp = *this;
    ++value;
    return tmp;
}
```

> Operator preinkrementacji może zmodyfikować `this` i zwrócić
referencję. Post inkrementacja nie może tego zrobić.

### Operator wywołania

Klasy mogą przeładować operator `(...)` z dowolną liczbą argumentów
powodując, że ich obiekty zachowują się jak funkcje.

```cpp
class LessThanFunc {
    int value;
public:
    /* ... */
    bool operator()(int x) {
        return x < value;
    }
    /* ... */
};
```
Source: [overloading_call.cpp](overloading_call.cpp)

Takie obiekty często nazywamy _funktorami_.

### Operator indeksowania

Można przeciążać operator `[]` dzięki czemu obiekty 
wyglądają jakby były indeksowalnymi kontenerami.

```cpp
class FakeArray {
public:
    /* ... */
    int operator[](std::size_t i) {
        return i + 1;
    }
};
```
Source: [overloading_index.cpp](overloading_index.cpp)

### Operator wyłuskania

Na potrzeby implementacji typów, których obiekty zachowują
się tak jakby były wskaźnikami można przeładować operator `->`.
Taki operator musi zwrócić wskaźnik (lub inny obiekt który)
ma przeładowany operator `->`).

```cpp
class Ptr
{
    Point* _ptr;
public:
    Ptr(Point* ptr) : _ptr(ptr) {}
    ~Ptr() { delete _ptr; }
    Point* operator->() { return _ptr; }
    Point& operator*() { return *_ptr; }
};
```

Przeciążamy również operator dereferencji `*` aby móc dobierać się 
do wskazywanego obiektu, a nie tylko do jego składowych.

## Konwersje

Implementując konstruktory, dostarczamy metody konstrukcji obiektów naszego typu.
Konstruktory jednoargumentowe są szczególne, bo pozwalają zamienić obiekt pewnego typu
na inny typ. To tak zwane **konstruktory konwertujące**. Taki konstruktor może zostać użyty niejawnie.

```cpp
class A {
public:
    A(int x);
};

void foo(A a);
void goo(const A& a);

foo(3); // ok!
goo(4)  // ok!
```

Kompilator automatycznie będzie stosował konwersje przy przekazywaniu zmiennych, np. do funkcji.
Takie niejawne konwersje mogą być trudne do zauważenia i niepożądane.
Jeżeli tak jest to jednoargumentowe konstruktory powinny być oznaczone słowem kluczowym `explicit`
blokującym niejawne konwersje.

```cpp
class A {
public:
    explicit A(int x);
};

void foo(A a);

// foo(3);               // błąd!
foo(A(3));               // ok!
foo(static_cast<A>(3));  // ok!
```

Konwersje można definiować również w formie **operatorów konwertujących**.
Klasa `T` może zdefiniować operator rzutowania na dowolny inny typ `U` (również wbudowany)
korzystając ze składni:

```cpp
class T {
operator U() { ... }
};

T t;
U u = static_cast<U>(t);
```

Typ zwracany takiego operatora to jego nazwa, więc nie jest powtarzana przed słowem kluczowym `operator`.
Operatory rzutowania podobnie do konstruktorów pozwalają na konwersje niejawne. Blokujemy je słowem kluczowym `explicit`.

Przykłady rzutowań: [conversion.cpp](conversion.cpp)

## Klasy zagnieżdżone

Klasy w C++ można zganieżdżać, czyli definiować/deklarować jedną wewnątrz drugiej. 
To użyteczne, jeżeli jakiś typ ma sens tylko w ścisłym związku z innym.
Typ zagnieżdżony może być dodatkowo prywatny, pozwalając na ukrywanie detali implementacyjnych otaczającej klasy.

```cpp
class Rectangle {
    struct Point {
        float x, y;
    };

    Point a, b;
public:
    Rectangle(float x0, float y0, float x1, float y1) : a {x0, y0}, b{x1, y1} {}

    float area() const {
        return (b.x - a.x) * (b.y - a.y);
    }
};
```
Source: [nested_classes.cpp](nested_classes.cpp)

Z klasy zagnieżdżonej `Point` może korzystać tylko klasa `Rectangle` w swojej implementacji.
Kompilator nie pozwoli korzystać z takiej klasy z zewnątrz: 

```cpp
Rectangle::Point p {1, 2}; // błąd!
```

Przykład: [nested_classes.cpp](nested_classes.cpp)

Klasy można też definiować w funkcjach (ogólniej w blokach `{...}`). Mają wtedy lokalny zakres użycia,
od definicji, do końca bloku.

## Iteratory

Rozważmy fragment kodu iterujący się po tablicy za pomocą wskażnika:

```cpp
int tab[] = {1, 2, 3, 4};
for (int* ptr = tab; ptr != tab + 4; ++ptr) {
    ...
}
```

Wskaźnik `ptr` pełni tutaj rolę obiektu pokazującego na element pewnego kontenera.
Programując obiektowo, często implementujemy klasy reprezentujące kolekcje obiektów,
czasami o znacznie bardziej złożonej strukturze wewnętrznej niż tablica.

```cpp
class list {
    struct node {
        int value;
        std::unique_ptr<node> next;
        node(int value, std::unique_ptr<node> next) : value(value), next(std::move(next)) {}
    };

    std::unique_ptr<node> head;
  public:
    list() = default;

    void prepend(int value) {
        head = std::make_unique<node>(value, std::move(head));
    }
};
```

Powyżej zaimplementowana lista pięknie ukrywa swoją wewnętrzną implementację.
Użytkownik nie ma dostępu do jej wewnętrznych wskaźników i typów, dzięki czemu operacja `prepend()`
i inne metody zapewnią spójność struktury danych. Skoro tak, to pozostaje pytanie: jak się po czymś takim _przeiterować_?

Można np. dostarczyć metodę `size()` i `get(int n)` pozwalającą dobierać się do
n-tego elementu, ale takie rozwiązanie nie jest optymalne ze względu na przeszukiwanie
listy od początku za każdym wywołaniem `get()`.

Musimy rozbudować interfejs klasy o coś, co pozwoli nam iterować się po kolekcji,
podobnie do wskaźnika wędrującego po tablicy. _To coś_ musi implementować logikę iteracji,
pamiętać stan iteracji: aktualne miejsce.

Kontener może zwrócić obiekt, specjalnej, dostarczonej przez siebie klasy.
Takie klasy i obiekty nazywamy **iteratorami**, uogólniającymi wskaźnik.

```cpp
class list {
    // ...
public:
    class iterator {
        node* ptr;
    public:
        iterator(node* ptr) : ptr(ptr) {}

        int operator*() { return ptr->value; }

        iterator& operator++() {
            ptr = ptr->next.get();
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) { return ptr == other.ptr; }
        bool operator!=(const iterator& other) { return !(*this == other); }
    };

    iterator begin() { return iterator(head.get()); }
    iterator end() { return iterator(nullptr); }

    //...
};
```
Source: [iterators_list.cpp](iterators_list.cpp)

Lista pozwala pozyskać iterator na początek i koniec, umożliwiając czytelną implementację iteracji:

```cpp
for (list::iterator it = l.begin(); it != l.end(); ++it) {
    std::cout << *it << std::endl;
}
```

Iterator implementuje logikę przechodzenia po kontenerze dostarczając operator `++`. Dzięki temu
wartości klasy `list::iterator` zachowują się podobnie do wskaźników.

Kontenery często dostarczają kilka różnych klas iteratorów, np. `reverse_iterator`, pozwalający
iterować się _od tyłu_. Typowo implementowane są typy `iterator` i `const_iterator`. Drugi nie pozwala modyfikować
wartości w kontenerze. 

```cpp
class vector {
    std::unique_ptr<int[]> _data;
    std::size_t _capacity;
    std::size_t _size;
  public:
    /* ... */
    
    class iterator {
        int* ptr;
        /* ... */
    };

    iterator begin() { return iterator(_data.get()); }
    iterator end() { return iterator(_data.get() + _size); }

    class const_iterator {
        const int* ptr;
        /* ... */
    };

    const_iterator cbegin() const { return const_iterator(_data.get()); }
    const_iterator cend() const { return const_iterator(_data.get() + _size); }

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }
};
```
Source: [iterators_vector.cpp](iterators_vector.cpp)

Dzięki drugiemu typowi można iterować się po kontenerze mając jego niemodyfikowalny `const` obiekt lub referencję na stałą.

## `auto`

Praca z przestrzeniami nazw, typami zagnieżdżonymi, w szczególności z iteratorami wymaga dużo pisania:

```cpp
utils::vector::const_iterator it = v.begin();
utils::vector::const_iterator end = v.end();
while (it != end) { ... }
```

Pisanie typu w definicjach obiektów `it`, `end` jest niejako powtarzaniem siebie. Widać przecież, co
zwracają metody `begin()` i `end()`.

Dlatego C++ pozwala używać słowa kluczowego `auto` w definicjach zmiennych w przypadku kiedy 
dostarczamy inicjalizator. Typ zmiennej zostanie automatycznie wydedukowany.

```cmake
class A {};

A get_A() { return A(); }

int main() {
    auto x = 1; // x is int
    auto a = A(); // a is A
    auto a2 = get_A(); // a2 is A

    auto& rx = x; // rx is int&
    auto& ra = a; // ra is A&

    const auto& rxa = a; // rxa is const A&
    const auto& rxa2 = get_A(); // rxa2 is const A&
    return 0;
}
```
Source: [auto.cpp](auto.cpp)

Mając `auto` korzystanie z iteratorów staje się przyjemne:

```cpp
for (auto it = l.begin(); it != l.end(); ++it) {
    std::cout << *it << std::endl;
}
```

## Ranged-for

C++ dostarcza jeszcze przyjemniejszą składnię iterowania się po kolekcjach.
Dla tablic oraz klas dostarczających iteratory zwracane przez `begin()` i `end()`
typowo stosuje się tzw. _ranged-for-loop_:

```cpp
list l;
// ...
for (int it : l) {
    std::cout << it << " ";
}
```
Source: [ranged_for.cpp](ranged_for.cpp)

Składnia uwalnia z konieczności ręcznego zarządzania zmiennymi iterującymi.
Wymaga podania jedynie typu elementu (może być `auto`/`auto&`) i kontenera.

```cpp
for (item_type item : collection) {
    // ...
}
```

## Standard Template Library

Biblioteka standardowa C++ zawiera zbiór klas i algorytmów pozwalających programom
łatwo manipulować abstrakcyjnymi strukturami danych takimi jak listy, słowniki, zbiory.
Te komponenty często nazywane są _standardową biblioteką szablonów_.

Jeden z podstawowych kontenerów STL już znamy: `std::vector<T>`. [Biblioteka](https://en.cppreference.com/w/cpp/container)
zawiera ich znacznie więcej:

* `std::array`: tablica stałego rozmiaru
* `std::vector`: tablica dynamiczna
* `std::list`: lista łączona dwukierunkowa
* `std::forward_list`: lista łączona jednokierunkowa 
* `std::deque`: kolejka dwukierunkowa

* `std::set`/`std::multiset`: zbiór uporządkowany
* `std::map`/`std::multimap`: słownik uporządkowany

* `std::unordered_set`/`std::unordered_multiset`: zbiór nieuporządkowany
* `std::unordered_map`/`std::unordered_multimap`: słownik nieuporządkowany 

* `std::stack`: stos
* `std::queue`: kolejka
* `std::priority_queue`: kolejka priorytetowa

Wszystkie kontenery przechowują obiekty zadanego typu,
dostarczają operatory, iteratory, funkcje modyfikujące ich stan.
Omówienie wszystkich byłoby niemożliwe i nużące. Do pracy z kontenerami STL trzeba korzystać z dokumentacji.
Omówimy kilka najbardziej istotnych przykładów.

