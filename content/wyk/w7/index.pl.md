---
title: "W7 - Metaprogramowanie"
date: 2022-02-05T18:00:18+01:00
weight: 70
---

# Wykład 7 - Metaprogramowanie

Zakres:
* function/class/variable template
* template specialization
* template partial specialization
* Koncepty
* `constexpr`, `consteval`, `constinit`

## Szablony


## Koncepty



## Obliczenia w czasie kompilacji

Rozważmy poniższy fragment kodu, który wykonuje obliczenia na stałych, znanych w czasie kompilacji:

```cpp
int add(int x, int y) { return x + y; }

int main(int argc, char** argv)
{
	const int x = 1;
	const int y = 2;
	const int result = 2 * x + y;
	
	int limit = add(result, x);
	
	if (argc > limit) {
		return 1;
	}
	
	return 0;
}
```
Source: [const.cpp](constexpr/const.cpp)

Takie obliczenia mógłby wykonać kompilator w trakcie tłumaczenia naszego programu.
Po co spowalniać i powiększać kod programu generując instrukcje mnożenia, dodawania, skoro można
od razu wyznaczyć wartości zmiennych `result` i `limit`. Technicznie, nic nie stoi na przeszkodzie,
żeby ten program w czasie wykonania wykonywał jedynie porównania `argc` ze stałą `limit`.
To jeden z podstawowych trików stosowanych przez optymalizator. Sprawdźmy to!

```shell
g++ constexpr/const.cpp -O0 -o const.debug
objdump --disassemble=main -M intel const.debug
```

Kompilując bez optymalizacji, nasz program będzie musiał bardzo się napracować.
Optymalizator co prawda obliczył wartość stałej `result` ale i tak wywołuje funkcję `add()`:

```text
0000000000001141 <main>:
    1141:       f3 0f 1e fa             endbr64
    1145:       55                      push   %rbp
    1146:       48 89 e5                mov    %rsp,%rbp
    1149:       48 83 ec 20             sub    $0x20,%rsp
    114d:       89 7d ec                mov    %edi,-0x14(%rbp)
    1150:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
    1154:       c7 45 f0 01 00 00 00    movl   $0x1,-0x10(%rbp)   # Stała x = 1
    115b:       c7 45 f4 02 00 00 00    movl   $0x2,-0xc(%rbp)    # Stała y = 2
    1162:       c7 45 f8 04 00 00 00    movl   $0x4,-0x8(%rbp)    # Stała result = 4
    1169:       be 01 00 00 00          mov    $0x1,%esi          # Drugi argument dla add()
    116e:       bf 04 00 00 00          mov    $0x4,%edi          # Pierwszy argument dla add()
    1173:       e8 b1 ff ff ff          call   1129 <_Z3addii>    # Wywołanie add(4, 1)
    1178:       89 45 fc                mov    %eax,-0x4(%rbp)    # Przypisanie limit = add(4, 1)
    117b:       8b 45 ec                mov    -0x14(%rbp),%eax
    117e:       3b 45 fc                cmp    -0x4(%rbp),%eax
    1181:       7e 07                   jle    118a <main+0x49>
    1183:       b8 01 00 00 00          mov    $0x1,%eax
    1188:       eb 05                   jmp    118f <main+0x4e>
    118a:       b8 00 00 00 00          mov    $0x0,%eax
    118f:       c9                      leave
    1190:       c3                      ret
```

Porównajmy to z włączonymi optymalizacjami:

```shell
g++ constexpr/const.cpp -DNDEBUG -O3 -o const.release
objdump --disassemble=main -M intel const.release
```

```text
0000000000001040 <main>:
    1040:       f3 0f 1e fa             endbr64
    1044:       31 c0                   xor    eax,eax
    1046:       83 ff 05                cmp    edi,0x5  # Porównanie argc > 5
    1049:       0f 9f c0                setg   al
    104c:       c3                      ret
```

I o to chodziło! Jedynie instrukcja porównania pozostała w skompilowanym kodzie.
Wszystkie stałe `x`, `y`, `result`, jak i nie-stała `limit` znikły, nie są nawet przechowywane w pamięci!

Jak widać słowo kluczowe `const` nie ma dla kompilatora w tym zakresie żadnego znaczenia. 
Jego jedynym zadaniem jest zapewnienie, żeby programista, pisząc kod C++, nie mógł zmodyfikować obiektu.
`const` nie gwarantuje w żaden sposób, że obliczenia zostaną wyoptymalizowane.
Nowsze standardy języka C++ dodają słowa kluczowe pozwalające jawnie tworzyć stałe czasu kompilacji i wykonywać
na nich obliczenia!

### `constexpr`

Słowo kluczowe `constexpr` pozwala deklarować stałe czasu kompilacji.
Pozwala również deklarować funkcje, które mogą być używane do obliczeń na takich stałych.

```cpp
constexpr int add(int x, int y) { return x + y; }

constexpr int x = 1;
constexpr int y = 2;
constexpr int result = 2 * x + y;

constexpr int limit = add(result, x);
```
Source: [constexpr.cpp](constexpr/constexpr.cpp)

Kwalifikując wszystkie stałe z powyższego przykładu słowem `constexpr`
mówimy, że nie tylko są `const`, to znaczy niemodyfikowalne w czasie wykonania,
ale też, że ich wartości mogą być obliczone przez kompilator.

Funkcje `constexpr` mogą być wołane zarówno jak zwykłe funkcje, w czasie wykonania,
jak i mogą być wykonywane przez kompilator, jeżeli ich argumenty są stałymi znanymi w czasie kompilacji.

Budując zmodyfikowany program, nawet bez obliczeń, zobaczymy, że kompilator obliczył wartość zmiennej `limit`,
nawet bez żadnych optymalizacji:

```shell
g++ constexpr/constexpr.cpp -O0 -o constexpr.debug
objdump --disassemble=main -M intel constexpr.debug
```

```
0000000000001129 <main>:
    1129:       f3 0f 1e fa             endbr64
    112d:       55                      push   rbp
    112e:       48 89 e5                mov    rbp,rsp
    1131:       89 7d ec                mov    DWORD PTR [rbp-0x14],edi
    1134:       48 89 75 e0             mov    QWORD PTR [rbp-0x20],rsi
    1138:       c7 45 f0 01 00 00 00    mov    DWORD PTR [rbp-0x10],0x1   # x = 1
    113f:       c7 45 f4 02 00 00 00    mov    DWORD PTR [rbp-0xc],0x2    # y = 2
    1146:       c7 45 f8 04 00 00 00    mov    DWORD PTR [rbp-0x8],0x4    # 2 * x
    114d:       c7 45 fc 05 00 00 00    mov    DWORD PTR [rbp-0x4],0x5    # limit = 2 * x + 1
    1154:       83 7d ec 05             cmp    DWORD PTR [rbp-0x14],0x5   # Porównanie argc < limit
    1158:       7e 07                   jle    1161 <main+0x38>
    115a:       b8 01 00 00 00          mov    eax,0x1
    115f:       eb 05                   jmp    1166 <main+0x3d>
    1161:       b8 00 00 00 00          mov    eax,0x0
    1166:       5d                      pop    rbp
    1167:       c3                      ret
```

Funkcje `constexpr` pierwotnie miały wiele ograniczeń. W pierwotnej wersji język pozwalał w ich ciele
tylko na jedną instrukcję: `return <wyrażenie>`. Obecnie możliwości są bardzo rozbudowane:



`constexpr` nie gwarantuje wykonania w czasie kompilacji, a pozwala na użycie stałych szerzej 
niż byłoby to możliwe z użyciem `const`.

```cpp

```

### `consteval`


