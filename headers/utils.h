#ifndef UTILS_H
#define UTILS_H
    #include "../headers/reader.h"
    #include "../headers/analyzer.h"
    #include "../headers/printer.h"
    typedef struct Reader_Utils Reader_Utils;
    typedef struct Analyzer_Utils Analyzer_Utils;
    typedef struct Printer_Utils Printer_Utils;

    Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd);
    Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
    Buffer* Reader_Utils_get_logger(Reader_Utils* utils);
    Reader* Reader_Utils_get_reader(Reader_Utils* utils);
    void Reader_Utils_destroy(Reader_Utils* utils);

    Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf);
    Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* utils);
    Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* utils);
    Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* utils);
    Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* utils);
    void Analyzer_Utils_destroy(Analyzer_Utils* utils);


    Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, int core_quantity);
    Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils);
    Buffer* Printer_Utils_get_logger(Printer_Utils* utils);
    int Printer_Utils_get_core_quantity(Printer_Utils* utils);
    void Printer_Utils_destroy(Printer_Utils* utils);
    

#endif

