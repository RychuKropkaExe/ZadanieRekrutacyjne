# ZadanieRekrutacyjne

Zadanie rekrutacyjne CUT - cpu usage tracker
Michał Kukowski
Kwiecień 2021
1 Wymagania:
- Projekt musi zostać napisany w języku C. W standardzie C99 lub wyższym. Przy czym należy napisać
kod tak, aby dało się go skompilować za pomocą gcc jak i clang. Kompilacja nie może zawierać ostrzeżeń!
(dla clang flaga -Weverything, dla gcc przynajmniej -Wall -Wextra),
- Projekt musi posiadać system budowania, najlepiej oparty na Makefile lub CMake. System powinien
wspierać dynamiczne zmiany kompilatora (czytanie zmiennej środowiskowej CC).
- Projekt musi posiadać system kontroli wersji, najlepiej git. Należy zamieścić ”ładną” historię commitów,
tak aby pokazać proces powstawania aplikacji,
- Aplikacja musi działać poprawnie na dowolnej distrybucji linuxa - np Ubuntu, Arch, Fedora, Debian,
- Aplikacja musi zostać przetestowana pod kątem wycieków pamięci, do tego należy użyć programu
valgrind
2 Opis zadania
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
1
3 Wskazówki
1. Do obliczania zużycia procesora można użyć tej formuły
2. Wysyłanie danych pomiędzy wątkami nie trzeba rozumieć dosłownie. Można użyć do tego globalnej
pamięci (np globalna tablica lub struktura).
3. Czytanie danych i wysyłanie do innego wątku to problem Producenta i konsumenta. Warto zatem
poczytać o tym problemie i przeanalizować jego rozwiązania.
4. Pomyśl o buforowaniu danych, co jeśli wątek czytający z pliku czyta szybciej niż przetwarzający?
Spróbuj rozwiązać ten problem za pomocą jakieś struktury danych (RingBuffer / Queue)
5. Wielowątkowość umożliwia nam znana biblioteka pthread lub nakładka na tę bibliotęke wbudowana w
język C11 i wyżej
6. Przechwytywanie sygnału SIGTERM może być wzorowane na tym przykładzie
7. Pamiętaj, że oceniamy nie tylko poprawność ale również styl programowania. Przejrzyj książke Nowoczesne C i zastosuj się do rad zawartych w tej książce. Pamiętaj, że język C jest tylko z pozoru trywialny,
pobaw się ficzerami C99 takimi jak VLA, FAM, Compund Literals. Jednak pamiętaj, ze skomplikowane
ficzery języka to broń obosieczna. Zanim czegoś użyjesz poznaj wady i zalety tego rozwiązania
8. Staraj się napisać kod w duchu paradygmatu obiektowego, zachowaj zasady KISS i SOLID w swoim
kodzie.
