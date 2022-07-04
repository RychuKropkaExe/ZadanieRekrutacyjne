
Zadanie rekrutacyjne do firmy Tietoevry
=
Autor: Józef Melańczuk

Treść:
```
Twoim zadaniem jest napisanie prostej aplikacji konsolowej do śledzenia zużycia procesora.
1. Pierwszy wątek (Reader) czyta /proc/stat i wysyła odczytany ciąg znaków (jako raw data) do wątku
drugiego (Analyzer)
2. Wątek drugi (Analyzer) przetwarza dane i wylicza zużycie procesa (wyrażone w %) dla każdego rdzenia
procesora widocznego w /proc/stat i wysyła przetworzone dane do wątku trzeciego (Printer)
3. Wątek trzeci (Printer) drukuje na ekranie w sposób sformatowany (format dowolny, ważne aby był
przejrzysty) średnie zużycie procesora co sekunde
4. Wątek czwarty (Watchdog) pilnuje aby program się nie zawiesił. Tzn jeśli wątki nie wyślą informacji
przez 2 sekundy o tym, że pracują to program kończy działanie z odpowiednim komunikatem błędu
5. Wątek piąty (Logger) przyjmuje wiadomości od wszystkich wątków i zapisuje wiadomości do pliku.
Loggera używa się do zapisywania debug printów do pliku w sposób zsynchronizowany
6. Należy także zaimplementować przechwytywanie sygnału SIGTERM i zadbać o odpowiednie zamknięcie
aplikacji (zamknięcie pliku, zwolnienie pamięci, zakończenie wątków)
Do poprawnego działania programu potrzebne są minimum wątki 1 (Reader), 2 (Analyzer) i 3 (Printer). Zatem należy zaimplementować minimum te funkcjonalności. Wątek 4 (Watchdog) i 5 (Logger) jak i
przechwytywanie sygnału są opcjonalne, jednak zachęcam do implementacji całości zadania.
```
Kompilacja:

Flagi: 
```c
CC=clang, by kompilować clangiem(domyślnie kompilacja używając gcc)
GFLAG=yes, by kompilować z flagą -ggdb3
```
Aplikacja:
```c
make
Wtedy należy uruchomić ją za pomocą ./tracker
```
Testy:
```c
make MODE=test
Należy je uruchomić za pomocą ./tests/tests
```
Zrealizowane podpunkty:

-Reader

-Analyzer

-Printer

-Logger

-Watchdog

-Przechwytywanie sygnału SIGTERM

Producent-Consumer problem:

W zadaniu napotykamy na problem współdzielenia zasobu przez wiele wątków, w tym przypadku są to bufory, za pomocą których przekazywane są informację. W moim podejściu skorzystałem z następującego ringed buffera, do komunikacji Reader-Analyzer, a także drugiej instancji tej struktury do komunikacji każdego z wątków z Loggerem:
```c
typedef struct Buffer
{   
    size_t size;
    size_t head;
    size_t tail;
    size_t max_size;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    Pack buffer[]; //FAM

} Buffer;
```

Analyzer z printerem komunikują się używając drugiego ringed buffer:
```c
typedef struct Results_buffer{
    size_t size;
    size_t head;
    size_t tail;
    size_t max_size;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    double buffer[]; //FAM
} Results_buffer;
```
By odciążyć sekcje krytyczne reader i analyzer przechowują dane w lokalnych magazynach





