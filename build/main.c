#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <stdbool.h>
#include "../headers/buffer.h"
#include "../headers/reader.h"
#include "../headers/utils.h"
#include "../headers/analyzer.h"
#include "../headers/printer.h"

int main(){
    
    int core_quantity = (int)sysconf(_SC_NPROCESSORS_ONLN);

    int time_interv = 1000000;

    Buffer* bf = Buffer_create((size_t)core_quantity+1);

    Reader* rd = Reader_create(time_interv, core_quantity);
    Reader_Utils* reader_utils = Reader_Utils_create(&bf,&rd);

    Analyzer* al = Analyzer_create(core_quantity);
    Results_buffer* rbf = Results_buffer_create((size_t)core_quantity+1);
    Analyzer_Utils* analyzer_utils = Analyzer_Utils_create(&bf,&al,&rbf);

    Printer_Utils* printer_utils = Printer_Utils_create(&rbf, core_quantity);

    pthread_t reader, analyzer, printer;
    pthread_create(&reader, NULL, reader_thread, (void*)&reader_utils);
    pthread_create(&analyzer, NULL, analyzer_thread, (void*)&analyzer_utils);
    pthread_create(&printer,NULL, printer_thread, (void*)&printer_utils);

    pthread_join(reader, NULL);
    pthread_join(analyzer, NULL);
    pthread_join(printer, NULL);


    Buffer_destroy(bf);
    Reader_destroy(rd);
    Results_buffer_destroy(rbf);
    Analyzer_destroy(al);
    Reader_Utils_destroy(reader_utils);
    Analyzer_Utils_destroy(analyzer_utils);
    Printer_Utils_destroy(printer_utils);

    return 0;
}
