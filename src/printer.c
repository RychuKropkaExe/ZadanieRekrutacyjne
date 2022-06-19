#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../headers/reader.h"
#include "../headers/analyzer.h"
#include "../headers/buffer.h"
#include "../headers/utils.h"


#define HISTOGRAM_WIDTH 20

void* printer_thread(void* args);


void* printer_thread(void* args) {
    Printer_Utils* utils = *(Printer_Utils**)args;
    Results_buffer* results_buffer = Printer_Utils_get_Results_buffer(utils);
    int core_quantity = Printer_Utils_get_core_quantity(utils);

    Buffer* logger = Printer_Utils_get_logger(utils);

    Log_message(logger,"[PRINTER][INFO] STARTING THREAD\n");

    while(true){
        Results_buffer_lock(results_buffer);
        if(Results_buffer_is_empty(results_buffer)){
            Results_buffer_wait_for_producer(results_buffer);
        }
        printf("--------------------------------------\n");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("TIME: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        for(int i = 0; i < core_quantity + 1; ++i){
            if(Results_buffer_is_empty(results_buffer))
                printf("UNCOMPLETE PACKAGE, ABORT!\n");
            
            double usage = Results_buffer_get(results_buffer);

            int number_of_blocks = (int)(usage/(100/HISTOGRAM_WIDTH));

            if(i != 0){
                printf("CPU%d USAGE[",i);
            } else{
                printf("CPU  USAGE[");
            }

            for(int j = 0; j < number_of_blocks; j++){
                printf("#");
            }

            for(int j = 0; j < HISTOGRAM_WIDTH - number_of_blocks; j++){
                printf(" ");
            }

            printf("]");
            printf(" %.1lf %% \n", usage);
        }
        printf("--------------------------------------\n");
        Results_buffer_call_producer(results_buffer);
        Results_buffer_unlock(results_buffer);
        Log_message(logger,"[PRINTER][INFO] SUCCESSFULY PRINTED DATA\n");
    }
    return NULL;
}

