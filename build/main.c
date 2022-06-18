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

int main(){
    int core_quantity = sysconf(_SC_NPROCESSORS_ONLN);

    int time_interv = 100000;
    Buffer* bf = Buffer_create(core_quantity+1);

    Reader* rd = Reader_create(time_interv, core_quantity);
    Reader_Utils* ut = Reader_Utils_create(&bf,&rd);

    Analyzer* al = Analyzer_create(core_quantity);
    Results_buffer* rbf = Results_buffer_create(core_quantity+1);
    Analyzer_Utils* ut2 = Analyzer_Utils_create(&bf,&al,&rbf);

    pthread_t prod, cons;
    pthread_create(&prod, NULL, reader_thread, (void*)&ut);
    pthread_create(&cons, NULL, analyzer_thread, (void*)&ut2);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    Buffer_destroy(bf);
    Reader_destroy(rd);
    Results_buffer_destroy(rbf);
    Reader_Utils_destroy(ut);
    Analyzer_Utils_destroy(ut2);
    return 0;
}