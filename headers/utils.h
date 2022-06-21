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

    Reader_Utils* Reader_Utils_create(Buffer* const restrict bf,Buffer* const restrict logger, Reader* const rd, Dog* const dog);
    Buffer* Reader_Utils_get_buffer(Reader_Utils* const utils);
    Buffer* Reader_Utils_get_logger(Reader_Utils* const utils);
    Reader* Reader_Utils_get_reader(Reader_Utils* const utils);
    Dog* Reader_Utils_get_dog(Reader_Utils* const utils);
    void Reader_Utils_destroy(Reader_Utils* const utils);

    Analyzer_Utils* Analyzer_Utils_create(Buffer* const restrict bf, Buffer* const restrict logger, Analyzer* const al, Results_buffer* const rbf, Dog* const dog);
    Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* const utils);
    Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* const utils);
    Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* const utils);
    Dog* Analyzer_Utils_get_dog(Analyzer_Utils* const utils);
    Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* const utils);
    void Analyzer_Utils_destroy(Analyzer_Utils* const utils);


    Printer_Utils*  Printer_Utils_create(Results_buffer* const rbf,Buffer* const logger, Dog* const dog,int core_quantity);
    Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* const utils);
    Buffer* Printer_Utils_get_logger(Printer_Utils* const utils);
    Dog* Printer_Utils_get_dog(Printer_Utils* const utils);
    int Printer_Utils_get_core_quantity(Printer_Utils* const utils);
    void Printer_Utils_destroy(Printer_Utils* const utils);


    Logger_Utils* Logger_Utils_create(Buffer* const logger,Dog* const dog);
    Buffer* Logger_Utils_get_logger(Logger_Utils* const utils);
    Dog* Logger_Utils_get_dog(Logger_Utils* const utils);
    void Logger_Utils_destroy(Logger_Utils* const utils);

    Watchdog_Utils* Watchdog_Utils_create(Watchdog* const watchdog, Buffer* const restrict logger, Buffer* const restrict bf, Results_buffer* const rbf);
    Watchdog* Watchdog_Utils_get_watchdog(Watchdog_Utils* utils);
    Buffer* Watchdog_Utils_get_buffer(Watchdog_Utils* const utils);
    Buffer* Watchdog_Utils_get_logger(Watchdog_Utils* const utils);
    Results_buffer* Watchdog_Utils_get_results_buffer(Watchdog_Utils* const utils);
    void Watchdog_Utils_destroy(Watchdog_Utils* const utils);

#endif

