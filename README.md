# Zadanie zaliczeniowe 3: Pętlik
## Wprowadzenie

Rozważamy realizację języka programowania Pętlik. Składnię języka opisuje gramatyka z symbolem początkowym `Program`:

```console
Program → CiągInstrukcji
CiągInstrukcji → ε | CiągInstrukcji Instrukcja
Instrukcja → Zwiększ | Powtarzaj
Zwiększ → Zmienna
Powtarzaj → '(' Zmienna CiągInstrukcji ')'
Zmienna → 'a' | .. | 'z'
```
Program składa się ze znaków, które w gramatyce są ujęte w apostrofy. Oprócz nich żadne inne znaki, nawet spacje lub końce wiersza, w kodzie źródłowym nie mogą wystąpić.

Program ma dostęp do `26` zmiennych, których wartości są nieujemnymi liczbami całkowitymi.

Instrukcje wyprowadzone z symbolu `CiągInstrukcji` są wykonywane w kolejności od pierwszej do ostatniej.

Instrukcja `Zwiększ` w postaci `zmienna` jest równoważna instrukcji języka C:

```c
++zmienna;
```
Instrukcja` Powtarzaj` w postaci `(zmienna...)`jest równoważna instrukcji języka C:

```c
while (zmienna > 0) {
    --zmienna;
    ...
}
```
Realizacja języka składa się z kompilatora optymalizującego, generującego kod na maszynę wirtualną, oraz z interpretera kodu tej maszyny.

Maszyna wykonuje instrukcje:

```asm
INC Zmienna (increment)
```

zwiększ o jeden wartość zmiennej Zmienna,

```asm
ADD Zmienna0 Zmienna1 (add)
```

dodaj do zmiennej Zmienna0 wartość zmiennej Zmienna1,

```asm
CLR Zmienna (clear)
```

wyzeruj zmienną Zmienna,

```asm
JMP Adres (jump)
```

skocz do instrukcji o adresie Adres,

```asm
DJZ Zmienna Adres (decrement or jump if zero)
```

jeśli `Zmienna` ma wartość `0` to skocz do instrukcji o adresie `Adres`, w przeciwnym przypadku zmniejsz o jeden wartość zmiennej `Zmienna`,

```asm
HLT (halt)
```

zakończ wykonywanie programu.

Wykonanie programu w języku maszynowym zaczyna się od pierwszej instrukcji.

Jeśli instrukcja nie określa inaczej, to po jej wykonaniu przechodzimy do instrukcji, która jest w kodzie bezpośrednio za nią.

Dla ciągu instrukcji kompilator generuje kod w kolejności od pierwszej do ostatniej. Kod wygenerowany dla programu kończy instrukcją HLT.

Jeżeli w instrukcji `Powtarzaj` nie jest zagnieżdżona inna instrukcja `Powtarzaj`, czyli w nawiasach jest ciąg zmiennych `Zmienna0, ..., ZmiennaN`, dla `N >= 0`, i jeżeli żadna ze zmiennych `Zmienna1, ..., ZmiennaN` nie jest zmienną `Zmienna0`, to kompilator generuje kod zoptymalizowany postaci:

```asm
ADD Zmienna1 Zmienna0
...
ADD ZmiennaN Zmienna0
CLR Zmienna0
```

Jeśli nie są spełnione warunki umożliwiające wygenerowanie dla instrukcji kodu zoptymalizowanego, to generujemy kod zwykły.

Kodem zwykłym dla instrukcji `Zwiększ` postaci `Zmienna` jest:

```asm
INC Zmienna
```

Kodem zwykłym dla instrukcji `Powtarzaj` postaci `(Zmienna...)` jest:

```asm
DJZ Zmienna Koniec
...
JMP Początek
```

gdzie `Początek` to adres instrukcji `DJZ` w tym ciągu a `Koniec` to adres instrukcji bezpośrednio za instrukcją `JMP`.

## Polecenie

Napisz program będący realizacją języka Pętlik.

Program czyta i wykonuje polecenia:

- wypisania wartości zmiennej,

- wykonania programu w języku Pętlik.

Przed wykonaniem pierwszego polecenia wartości wszystkich zmiennych są równe `0`.

Zmienne zachowują wartości po wykonaniu polecenia. Nie są zerowane przed każdym wykonaniem programu.

## Postać danych

Dane programu to ciąg wierszy, w każdym po jednym poleceniu.

Polecenie wypisania wartości zmiennej zaczyna się od znaku `=`, po którym jest nazwa zmiennej.

Wiersz polecenia wykonania programu zawiera kod źródłowy w języku Pętlik.

## Postać wyniku

Wynik programu jest efektem wykonania poleceń wypisania wartości zmiennej.

Wartość zmiennej piszemy dziesiętnie, w jednym wierszu, bez nieznaczących zer wiodących.

Wartość `0` jest reprezentowana przez cyfrę `0` a wartości dodatnie zaczynają się od cyfry niezerowej.

## Przykłady

Do treści zadania dołączone są pliki `.in` z przykładowymi danymi i pliki `.out` z wynikami wzorcowymi.

- Dla danych `przyklad1.in` poprawny wynik to `przyklad1.out` .

- Dla danych `przyklad2.in` poprawny wynik to `przyklad2.out` .

- Dla danych `przyklad3.in` poprawny wynik to `przyklad3.out` .

## Walidacja i testy

- Rozwiązania zostaną poddane walidacji, wstępnie badającej zgodność ze specyfikacją.

Walidacja sprawdzi działanie programu na przykładach dołączonych do treści zadania.

Pomyślne przejście walidacji jest warunkiem dopuszczenia programu do testów poprawności. Program, który walidacji nie przejdzie, dostanie zerową ocenę poprawności.

- Walidacja i testy zostaną przeprowadzone na komputerze `students`.

- Programy będą kompilowane poleceniem:
```console
gcc @opcje nazwa.c -o nazwa
```

gdzie `nazwa.c` to nazwa pliku z kodem źródłowym a plik `opcje` ma zawartość:

```console
-std=c17
-pedantic
-Wall
-Wextra
-Wformat-security
-Wduplicated-cond
-Wfloat-equal
-Wshadow
-Wconversion
-Wjump-misses-init
-Wlogical-not-parentheses
-Wnull-dereference
-Wvla
-Werror
-fstack-protector-strong
-fsanitize=undefined
-fno-sanitize-recover
-g
-fno-omit-frame-pointer
-O1
```

Opcje `-std=c17`, `-pedantic` wskazują, że kompilator ma dbać o zgodność kodu z aktualnym standardem języka C.

Dzięki opcjom `-Wall`, `-Wextra` kompilator zgłosi zauważone usterki.

Opcje `-Wformat-security`, `-Wduplicated-cond`, `-Wfloat-equal`, `-Wshadow`, `-Wconversion`, `-Wjump-misses-init`, `-Wlogical-not-parentheses`, `-Wnull-dereference` umożliwiają wykrywanie dodatkowych usterek.

Opcja `-Wvla` sprawia, że użycie tablic zmiennej długości jest uznawane za usterkę.

Opcja `-Werror` wskazuje, że kompilator ma uznać usterki za błędy.

Dzięki opcji `-fstack-protector-strong`, podczas wykonania programu zostaną wykryte niektóre błędne odwołania do pamięci na stosie.

Opcje `-fsanitize=undefined`, `-fno-sanitize-recover` umożliwiają wykrywanie operacji, które mają efekt nieokreślony.

Opcje `-g`, `-fno-omit-frame-pointer` poprawiają jakość komunikatów o błędach wykonania.

Opcja `-O1` włącza optymalizacje, co zwiększa prawdopodobieństwo ujawnienia się błędów.

Wymagane są wszystkie wymienione opcje kompilatora. Nie będą do nich dodawane żadne inne.

Zwracamy uwagę, że poszczególne wersje kompilatora `gcc` mogą się różnić sposobem obsługi tych samych opcji. Przed wysłaniem rozwiązania warto więc skompilować je i przetestować na `students` w sposób opisany powyżej.

- Podczas walidacji i testów, program `nazwa` będzie uruchamiany pod kontrolą programu Valgrind poleceniem:

```console
valgrind --leak-check=full -q --error-exitcode=1 ./nazwa
```

Jeśli Valgrind wykryje błąd, to nawet, gdyby wynik był prawidłowy uznamy, że program testu nie przeszedł.

Opcja `-q` powoduje, że jedynymi informacjami, wypisywanymi przez program Valgrind, są komunikaty o błędach.

Opcja `--leak-check=full` wskazuje Valgrindowi, że powinien, między innymi, szukać wycieków pamięci.

Opcja `--error-exitcode=1` określa kod wyjścia programu w przypadku, gdy Valgrind wykryje błąd.

- Przyjmujemy, że niezerowy wynik funkcji `main()` informuje o błędzie wykonania programu.

- Poprawność wyniku sprawdzamy, przekierowując na wejście programu zawartość pliku `.in` i porównując rezultat, za pomocą programu `diff`, z plikiem `.out`, np.:

```console
< przyklad.in ./nazwa | diff - przyklad.out
```
Ocena poprawności wyniku jest binarna. Uznajemy go za poprawny, jeżeli program `diff` nie wypisze żadnej różnicy między wynikiem programu a wynikiem wzorcowym.

## Uwagi i wskazówki

- Jako rozwiązanie należy wysłać plik tekstowy `.c` z kodem źródłowym w języku C.

- Wolno założyć, że dane są poprawne.

- Wolno założyć, że każdy wiersz danych, w tym ostatni, będzie zakończony reprezentacją końca wiersza `'\n'`.

- Należy zadbać, by warunek ten spełniał także wynik programu.

- Wolno założyć, że podczas pracy programu nie nastąpi przepełnienie pamięci, czyli że wynikiem funkcji `malloc()` lub `realloc()`, wywołanej z odpowiednimi argumentami, nie będzie `NULL`.

- Wolno założyć, że długość programu w języku `Pętlik` nie przekroczy `INT_MAX - 1`.

- Wolno założyć, że wartości zmiennych, zapisane w układzie o podstawie `10000`, będą miały mniej niż `INT_MAX` cyfr.

- Nie wolno nakładać dodatkowych ograniczeń na długość programu i wartości zmiennych.

- W szczególności nie można zakładać, że wartości zmiennych będą w zakresie któregokolwiek z wbudowanych typów języka C, np. typu `int`.

- Efektywność rozwiązania może mieć wpływ na ocenę jego jakości.

- Rozwiązanie rażąco nieefektywne może nie przejść testów, a nawet walidacji, z powodu przekroczenia limitu czasu.

- Pod Linuxem, pracując z programem interakcyjnie na konsoli, koniec danych sygnalizujemy, naciskając klawisze `Ctrl-D`.

- W przygotowaniu danych testowych może pomóc polecenie `tee`. Przesyła ono dane z wejścia na wyjście, jednocześnie zapisując ich kopię w pliku, którego nazwa jest argumentem polecenia.

Wykonanie polecenia:

```sh
tee test.in | ./petlik
```
uruchomi program `petlik`, przekazując mu na wejście i jednocześnie zapisując do pliku `dane.in`, to, co wpisze użytkownik.

Test na tych samych danych będzie można powtórzyć poleceniem:
```sh
< test.in ./petlik > test.out
```

