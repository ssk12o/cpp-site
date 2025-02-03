---
title: "W2 - Pamięć"
date: 2022-02-05T18:00:18+01:00
weight: 20
---

# Wykład 2 - Pamięć

typesystem, typy wbudowane, structy, enumy, strong enumy, operatory wbudowane, wskaźniki i referencje, własności const/volatile,  heap, stack, new/delete, ASAN

### Obiekty

Programy w C++ manipulują [obiektami](https://en.cppreference.com/w/cpp/language/object).
Obiekty żyją w pamięci programu.
Cechy obiektów:
* rozmiar (`sizeof()`)
* wyrównanie (`alignof()`)
* trwałość pamięci (automatyczna, statyczna, dynamiczna, thread-local)
    * określa zasady kiedy pamięć na obiekt będzie pozyskiwana i zwalniana
* czas życia
    * jasno określone punkty początku i końca życia obiektu w czasie wykonania programu
* typ
* wartość
* nazwa (opcjonalna)

Zadeklarowane obiekty (i referencje) nazywamy **zmiennymi**.

Przykłady obiektów:
```cpp
long global;

void foo() {
    int uninit;
    int x = 3;
    vector<int> v;
    int* ptr = &x;
}

void goo() {
    int* ptr = new int(3);
    // ...
    delete ptr;
}
```
