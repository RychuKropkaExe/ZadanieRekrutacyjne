#ifndef UTILS_TESTS_H
#define UTILS_TESTS_H
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include "../headers/reader.h"
    #include "../headers/analyzer.h"
    #include "../headers/buffer.h"
    #include "../headers/utils.h"
    #include "../headers/watchdog.h"

    void utils_test(void);

    void utils_test(void){
        int time_interv = 100;
        int core_quantity = 8;
        Watchdog* pack = Watchdog_create();
        Dog* reader_dog = Watchdog_get_reader(pack);
        Dog* analyzer_dog = Watchdog_get_analyzer(pack);
        Dog* printer_dog = Watchdog_get_printer(pack);
        Dog* logger_dog = Watchdog_get_logger(pack);

        Buffer* bf = Buffer_create((size_t)core_quantity+1);
        Buffer* log = Buffer_create(20);

        Reader* rd = Reader_create(time_interv, core_quantity);
        Reader_Utils* reader_utils = Reader_Utils_create(bf, log, rd, reader_dog);

        Analyzer* al = Analyzer_create(core_quantity);
        Results_buffer* rbf = Results_buffer_create((size_t)core_quantity+1);
        Analyzer_Utils* analyzer_utils = Analyzer_Utils_create(bf,log, al,rbf, analyzer_dog);

        Printer_Utils* printer_utils = Printer_Utils_create(rbf, log, printer_dog, core_quantity);

        Logger_Utils* logger_utils = Logger_Utils_create(log,logger_dog);

        Watchdog_Utils* watchdog_utils = Watchdog_Utils_create(pack,log,bf,rbf);

        assert(reader_utils != NULL);
        assert(analyzer_utils != NULL);
        assert(printer_utils != NULL);
        assert(logger_utils != NULL);
        assert(watchdog_utils != NULL);

        assert(bf == Reader_Utils_get_buffer(reader_utils));
        assert(bf == Analyzer_Utils_get_buffer(analyzer_utils));
        assert(bf == Watchdog_Utils_get_buffer(watchdog_utils));

        assert(rbf == Analyzer_Utils_get_Results_buffer(analyzer_utils));
        assert(rbf == Printer_Utils_get_Results_buffer(printer_utils));
        assert(rbf == Watchdog_Utils_get_results_buffer(watchdog_utils));

        assert(log == Reader_Utils_get_logger(reader_utils));
        assert(log == Analyzer_Utils_get_logger(analyzer_utils));
        assert(log == Printer_Utils_get_logger(printer_utils));
        assert(log == Watchdog_Utils_get_logger(watchdog_utils));

        assert(reader_dog == Reader_Utils_get_dog(reader_utils));
        assert(analyzer_dog == Analyzer_Utils_get_dog(analyzer_utils));
        assert(printer_dog == Printer_Utils_get_dog(printer_utils));
        assert(logger_dog == Logger_Utils_get_dog(logger_utils));

        assert(rd == Reader_Utils_get_reader(reader_utils));
        assert(al == Analyzer_Utils_get_analyzer(analyzer_utils));

        Buffer_destroy(bf);
        Buffer_destroy(log);
        Reader_destroy(rd);
        Results_buffer_destroy(rbf);
        Analyzer_destroy(al);
        Reader_Utils_destroy(reader_utils);
        Analyzer_Utils_destroy(analyzer_utils);
        Printer_Utils_destroy(printer_utils);
        Logger_Utils_destroy(logger_utils);
        Watchdog_Utils_destroy(watchdog_utils);
        Watchdog_destroy(pack);
        printf("PASSED UTILS TEST\n");
    }


#endif
