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
    Logger_Utils* utils = *(Logger_Utils**)args;
    Buffer* logger = Logger_Utils_get_logger(utils);
    Dog* dog = Logger_Utils_get_dog(utils);
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    char file_name[80];

    int hours, minutes, seconds, day, month, year;
    hours = local->tm_hour;        
    minutes = local->tm_min;        
    seconds = local->tm_sec;        
 
    day = local->tm_mday;            
    month = local->tm_mon + 1;    
    year = local->tm_year + 1900;
    snprintf(file_name,80,"%s%d-%02d:%02d:%02d_%02d:%02d%s","./logs/Logs",year,month,day,hours,minutes,seconds, ".log");
    FILE* file = fopen(file_name,"w");
    if(file == NULL) {
       exit_on_error("[LOGGER][ERROR] COULD NOT OPEN A FILE\n");
    }

    Pack* pc = NULL;
    char* buffer_get = NULL;

    fwrite("[LOGGER][INFO] LOGS CREATED\n",sizeof(char),strlen("[LOGGER][INFO] LOGS CREATED\n"),file);
    while(Dog_get_flag(dog)){
        char log[256];
        Buffer_lock(logger);
        if(Buffer_is_empty(logger) && Dog_get_flag(dog)){
            Buffer_wait_for_producer(logger);
        }

        if(!Dog_get_flag(dog)){
            Buffer_unlock(logger);
            break;
        }

        pc = Buffer_get(logger);
        buffer_get = Pack_get_content(pc);
        memcpy(&log[0],buffer_get,256);

        fwrite(log,sizeof(char),strlen(log),file);
        if(fflush(file) != 0){
            exit_on_error("[LOGGER][ERROR] COULD NOT FLUSH A FILE\n");
            free(buffer_get);
            break;
        }
        Buffer_call_producer(logger);
        Buffer_unlock(logger);
        free(buffer_get);
        Dog_kick(dog);
    }
    fwrite("[LOGGER][INFO] LOGS CREATED\n",sizeof(char),strlen("[LOGGER][INFO] LOGS CREATED\n"),file);
    fclose(file);
    return NULL;
}