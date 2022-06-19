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

void* logger_thread(void* args);

void* logger_thread(void* args){
    Buffer* logger = *(Buffer**)args;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char file_name[40];
    sprintf(file_name,"%s%d-%02d-%02d-%02d:%02d:%02d%s", "./logs/Logs",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ".log");
    FILE* file = fopen(file_name,"w");
    if(file == NULL) {
        printf("ERROR WHILE OPENING A FILE\n");
    }

    Pack* pc = NULL;
    char* buffer_get = NULL;

    fwrite("[LOGGER][INFO] LOGS CREATED\n",sizeof(char),strlen("[LOGGER][INFO] LOGS CREATED\n"),file);
    while(true){
        char log[256];

        Buffer_lock(logger);
        if(Buffer_is_empty(logger)){
            Buffer_wait_for_producer(logger);
        }
        pc = Buffer_get(logger);
        buffer_get = Pack_get_content(pc);
        memcpy(&log[0],buffer_get,256);

        fwrite(log,sizeof(char),strlen(log),file);
        if(fflush(file) != 0){
            printf("ERROR WHILE FLUSHING A FILE!\n");
        }
        Buffer_call_producer(logger);
        Buffer_unlock(logger);
        free(buffer_get);
        //free(pc);

    }
    fclose(file);
    return NULL;
}