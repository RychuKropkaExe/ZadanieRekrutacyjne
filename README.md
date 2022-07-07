
Zadanie rekrytacyjne firmy Tietoevry

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
make (tutaj ewentualne flagi)
Wtedy należy uruchomić ją za pomocą ./tracker
```
Testy:
```c
make MODE=test (tutaj ewentualne flagi)
Należy je uruchomić za pomocą ./tests/tests
```
Zrealizowane podpunkty:

-Reader

-Analyzer

-Printer

-Logger

-Watchdog

-Przechwytywanie sygnału SIGTERM

#### Producent-Consumer problem:

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
By odciążyć sekcje krytyczne reader i analyzer przechowują dane w lokalnych magazynach:

```c
typedef struct Local_storage{
    size_t size;
    size_t head;
    size_t tail;
    Pack storage[]; //FAM
} Local_storage;
```
On nie posiada zabezpieczeń przeciwko wielowątkowości, gdyż nie jest współdzielony przez żadne z wątków.

#### Reader:
Wątek ten nie posiada żadnej szczególnej magi w sobie, po prostu czyta zawaratość /proc/stat, zapisuje ją w lokalnym magazynie i wysył ją bufferem do analyzera.

#### Analyzer:
Analyzer przetwarza otrzymane od readera dane. Najpierw zapisuje je w lokalnym magazynie, by zwolnić dostęp do buffera, a następnie przetwarza te dane. Do wyciągnięcia ogólnego zużycia procesora używa tej formuły:
```
PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
```

Tak przetworzone dane wysyła do Printera, używając Result_Buffer-a.

#### Printer:
Printer wyciąga przetworzone dane z Result_Buffer-a i drukuje je na terminalu, w pewien "Upiększony" sposób. Jest to kwestia estetyczna, więc nie dotyczy funkcjonalności samego programu.

#### Logi:
Do logów wykorzystałem drugą instancję Buffer-a opisanego na początku
Jest to buffer współdzielony przez wszystkie wątki, z którego po prostu wyciąga i drukuje do pliku otrzymane wiadomości. Nie składa ich, wszystkie komunikaty są wpisywane ręcznie. Program jest zabezpieczony przed overflowem w tym aspekcie.
Sam logger kończy, domyślnie, pracę jako ostatni, gdyż potrzebuje najpierw wyczyścić buffer z zalegających wiadomości, które mogłyby zostać nadesłane dopiero po zakończeniu jego pracy, stąd wyjście jako ostatni.

#### Watchdog:

Watchdog to wątek który sprawdza czy program "żyje", w skrócie czy żaden wątek się nie zawiesił. Robi to korzystając z alarmów(zwanych "psami"), które po każdej iteracji wątki muszą odnowić. Co każde 2 sekundy watchdog sprawia, czy te alarmy zostały odnowione, jeżeli nie, to kończy pracę program. Oczywiście robi to w taki sposób, by pamięć została zwolniona, a pliki zamknięte(jeżeli to możliwe). Sam Watchdog jest prosty w swojej budowie i operuje na dwóch strukturach:
```c
typedef struct Watchdog{
    Dog* reader;
    Dog* analyzer;
    Dog* printer;
    Dog* logger;
} Watchdog;

typedef struct Dog{
    pthread_mutex_t mutex;
    char pad[6];
    bool flag;
    bool is_alive;
} Dog;
```
"pies" posiada dwie flagi: "flag" mówiącą po prostu czy wątek dalej powinien wykonywać swoją pracę, oraz "is_alive", którą co każdą iterację wątek ustawia na prawdę, podczas gdy watchdog sprawdzając, czy wątki są aktywne ustawia ją na fałsz.
Watchdog kontroluje wszystkie "psy", a każdy wątek współdzieli z nim swojego "psa", więc wątki mogą bardzo często korzystać z tej struktury gdyż watchdog dostęp do niej potrzebuje jedynie co 2 sekundy. Watchdog także jest w stanie wykryć na jakiej linii nastąpiła stagnacja(może nie być w stanie podać dokładnego wątku, gdyż gdy zawiesi się reader, to analyzer też z jego perspektywy jest martwy). 


#### Przechwytywanie sygnału SIGTERM:
Samo przechwycenie sygnału jest w zasadzie bardzo proste:
```c
struct sigaction action;
memset(&action, 0, sizeof(action));
action.sa_handler = exit_gracefully;
sigaction(SIGTERM, &action, NULL);
sigaction(SIGINT, &action, NULL);
```

funkcja exit_gracefully to ta sama funkcja która jest używana przez Watchdoga, do zakończenia programu, więc nie ma tutaj nowego rozwiązani, tylko korzystam ze sprawdzonej już wcześniej funkcjonalności.







