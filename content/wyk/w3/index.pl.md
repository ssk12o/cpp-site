---
title: "W3 - Klasy"
date: 2022-02-05T18:00:18+01:00
weight: 30
---

# Wykład 3 - Klasy

Zakres:
* cechy programowania obiektowego
* klasy
* konstruktory i destruktory
* RAII
* konstruktory kopiujące i operatory przypisania
* konstruktory przenoszące i operatory przeniesienia
* reguła 3-ch, reguła 5-ciu
* widoczność
* przeciążanie operatorów
* konwersje
* literały zdefiniowane przez użytkownika
* składowe statyczne

### Programowanie Obiektowe

Programy obiektowe manipulują zbiorem obiektów, które mogą
się ze sobą komunikować. Obiekty łączą w sobie stan i zachowanie, 
to definiująca je cecha.
Stan to najczęściej dane przechowywane w pamięci obiektu, czyli pola.
Zachowanie modelujemy za pomocą funkcji obiektem, czyli metod.

> Nie należy łączyć tak samo brzmiących pojęć: _obiekt_ języka C++
> i _obiekt_ w paradygmacie programowania obiektowego.

Język programowanie nie musi zawierać mechanizmów wspierających
programowanie obiektowe. W C też można napisać program obiektowy
i często się to robi:

```c
struct Vector2f { float x, y; }; // definicja klasy

struct Vector2f make_vector(float x, float y) // konstruktor
{
    struct Vector2f v = {x, y};
    return v;
}

float vector2f_length(const struct Vector2f* this) // metoda
{
    return (float)sqrt(this->x * this->x + this->y * this->y);
}

struct Vector2f vector2f_add(struct Vector2f* this, const struct Vector2f* other) // metoda z argumentem
{
    this->x += other->x;
    this->y += other->y;
    return *this;
}
```

W tym przykładzie typ `Vector2f` łączy w sobie 2 pola danych: `x` i `y` oraz dostarcza interfejs
modyfikujący i odczytujący ten stan w postaci dwóch funkcji, z których każda operuje na instancji typu `Vector2f`.

### Cechy programowania obiektowe

#### Abstrakcja

Polega na ukrywaniu zbędnych szczegółów implementacyjnych i prezentowaniu jedynie istotnych cech obiektu.
Dzięki abstrakcji obiekty mają zrozumiały dla programisty interfejs, odzwierciedlający rzeczywiste obiekty 
lub zjawiska. Użytkownik operuje na interfejsie, a szczegóły techniczne są ukryte.

#### Hermetyzacja (Enkapsulacja)

To cecha polegająca na ukrywaniu wewnętrznego stanu obiektu oraz dostarczaniu kontrolowanego dostępu do niego przez metody 
(np. _gettery_ i _settery_). Hermetyzacja chroni przed bezpośrednią ingerencją w strukturę obiektu i pozwala na większą kontrolę nad jego spójnością.

#### Dziedziczenie

Umożliwia tworzenie nowych typów obiektu, na podstawie istniejących, 
dzięki czemu można ponownie wykorzystać kod oraz dodawać lub modyfikować funkcjonalności. 
Typ bazowy udostępnia pola i metody typom dziedziczącym (klasom pochodnym), 
co poprawia organizację i minimalizuje redundancję kodu.

#### Polimorfizm (Wielopostaciowość)

Pozwala na różne zachowanie obiektów w zależności od ich konkretnego typu, 
korzystając z tego samego interfejsu. Wybór konkretnej implementacji zachowania
może zachodzić w czasie kompilacji, np. wybór funkcji przeciążonej, wybór operatora,
lub co bardziej charakterystyczne, w czasie wykonania programu.

> Te 4 cechy to bardzo często zadawane pytania na obronie :)

Użytkownik typu `Vector2f` nie musi wnikać w to, jak są przechowywane składowe wektora. Na stosie, stercie,
w tablicy w niezależnych zmiennych. To bez znaczenia, bo posługuje się jasno określonym interfejsem
`make_vector`. Mamy abstrakcję.

Na tym przykładzie widać, że C nie dostarcza dobrej hermetyzacji.
Każdy może ręcznie dobrać się do pól obiektu i je zmodyfikować, potencjalnie wprowadzając w niepoprawny stan.
Hermetyzacji brakuje. Możnaby ukryć pola korzystając z idiomu _pointer to the implementation_:

```c
// vector2f.h - publiczny nagłówek

struct _vector2f_impl; // coś ukrytego
struct Vector2f { _vector2f_impl* _impl; };

float vector2f_length(const struct Vector2f* this);

// impl.c - prywatna jednostka translacji

struct _vector2f_impl { float x, y; }

float vector2f_length(const struct Vector2f* this) {
    // tu widać czym _vector2f_impl jest
    return (float)sqrt(this->_impl->x * this->_impl->x + this->_impl->y * this->_impl->y);
}
```

Niewygodne, ale można.

Może wydawać się, że dziedziczenie i polimorfizm nie są w C do zrealizowania, ale to nieprawda!
Do reazliacji dziedziczenia wystarczy zagnieżdżać struktury bazowe w pochodnych:
```c
struct Shape { char name[10]; }
struct Circle { Shape base; float cx, cy, r; }
struct Square { Shape base; float x1, y1, x2, y2; }
```

Struktury dziedziczące, re-używają pól struktury bazowej, a ponieważ `Circle*` można rzutować na `Shape*` 
(bo `Shape` to pierwszy pod-obiekt) to funkcje dostarczane przez `Shape`, oczekujące `Shape*` będą działać też
z typami pochodnymi.

Polimorfizm dynamiczny, czyli wybór implementacji zachowania w zależności od typu można zrealizować,
przechowując w strukturze wskaźniki na funkcje. Wskaźniki będą ustawione na różne wartości
w zależności od konkretnego typu obiektu.

```c
struct Shape;
typedef float (*area_func_t)(const Shape*); 
struct Shape {
    char name[10];
    area_func_t f_area; // funkcja obliczająca pole zależna od konkretnego kształtu
}

float shape_area(Shape* s) {
    return s.f_area(s); // Skok do dynamicznie wybranej implementacji
}
```

Warto spojrzeć na kompleksowy przykład: [cobj.c](cobj.c)






