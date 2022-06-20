#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "../headers/utils.h"



int main(){

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = exit_gracefully;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);


    int core_quantity = (int)sysconf(_SC_NPROCESSORS_ONLN);

    int time_interv = 1000000;

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

    pthread_t reader, analyzer, printer, logger, watchdog;
    pthread_create(&reader, NULL, reader_thread, (void*)&reader_utils);
    pthread_create(&analyzer, NULL, analyzer_thread, (void*)&analyzer_utils);
    pthread_create(&printer,NULL, printer_thread, (void*)&printer_utils);
    pthread_create(&logger,NULL, logger_thread, (void*)&logger_utils);
    pthread_create(&watchdog,NULL, watchdog_thread, (void*)&watchdog_utils);


    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);
    pthread_join(logger,NULL);
    pthread_join(watchdog,NULL);


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

    return 0;
}
