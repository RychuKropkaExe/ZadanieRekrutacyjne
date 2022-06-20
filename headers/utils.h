#ifndef UTILS_H
#define UTILS_H
    #include "../headers/reader.h"
    #include "../headers/analyzer.h"
    #include "../headers/printer.h"
    #include "../headers/watchdog.h"
    #include "../headers/logger.h"
    #include "../headers/buffer.h"
    
    typedef struct Reader_Utils Reader_Utils;
    typedef struct Analyzer_Utils Analyzer_Utils;
    typedef struct Printer_Utils Printer_Utils;
    typedef struct Logger_Utils Logger_Utils;
    typedef struct Watchdog_Utils Watchdog_Utils;

    Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd, Dog* dog);
    Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
    Buffer* Reader_Utils_get_logger(Reader_Utils* utils);
    Reader* Reader_Utils_get_reader(Reader_Utils* utils);
    Dog* Reader_Utils_get_dog(Reader_Utils* utils);
    void Reader_Utils_destroy(Reader_Utils* utils);

    Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf, Dog* dog);
    Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* utils);
    Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* utils);
    Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* utils);
    Dog* Analyzer_Utils_get_dog(Analyzer_Utils* utils);
    Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* utils);
    void Analyzer_Utils_destroy(Analyzer_Utils* utils);


    Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, Dog* dog,int core_quantity);
    Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils);
    Buffer* Printer_Utils_get_logger(Printer_Utils* utils);
    Dog* Printer_Utils_get_dog(Printer_Utils* utils);
    int Printer_Utils_get_core_quantity(Printer_Utils* utils);
    void Printer_Utils_destroy(Printer_Utils* utils);


    Logger_Utils* Logger_Utils_create(Buffer* logger, Dog* dog);
    Buffer* Logger_Utils_get_logger(Logger_Utils* utils);
    Dog* Logger_Utils_get_dog(Logger_Utils* utils);
    void Logger_Utils_destroy(Logger_Utils* utils);


    Watchdog_Utils* Watchdog_Utils_create(Watchdog* watchdog, Buffer* logger, Buffer* bf, Results_buffer* rbf);
    Watchdog* Watchdog_Utils_get_watchdog(Watchdog_Utils* utils);
    Buffer* Watchdog_Utils_get_buffer(Watchdog_Utils* utils);
    Buffer* Watchdog_Utils_get_logger(Watchdog_Utils* utils);
    Results_buffer* Watchdog_Utils_get_results_buffer(Watchdog_Utils* utils);
    void Watchdog_Utils_destroy(Watchdog_Utils* utils);

#endif

