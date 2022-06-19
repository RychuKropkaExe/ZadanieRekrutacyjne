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

#define PREFIX "["

void* logger_thread(void* args);

void* logger_thread(void* args){
    Buffer* logger = *(Buffer**)args;
    FILE* file = fopen("logs.log","w");
    if(file == NULL) {
        printf("ERROR WHILE OPENING A FILE\n");
    }
    while(true){
        char log[256];
        Buffer_lock(logger);
        if(Buffer_is_empty(logger)){
            Buffer_wait_for_producer(logger);
        }
        char* buffer_get = Buffer_get(logger);
        memcpy(&log[0],buffer_get,256);
        fwrite(log,sizeof(char),strlen(log),file);
        free(buffer_get);
        Buffer_call_producer(logger);
        Buffer_unlock(logger);
    }
    return NULL;
}