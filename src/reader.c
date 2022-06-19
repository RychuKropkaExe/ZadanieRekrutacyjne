#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../headers/reader.h"
#include "../headers/buffer.h"
#include "../headers/utils.h"
//Maksymalna ilość bajtów w paczce, jedna linia z /proc/stat nie przekroczy tej wartości
//A taki rozmiar, jest raczej bardzo pesymistycznym górnym ograniczeniem
#define MAX_SIZE 256

typedef struct Reader{
    int time_interv;
    int core_quantity;
} Reader;

Reader* Reader_create(int time_int, int core_quantity);
void Reader_destroy(Reader* rd);
int Reader_get_time(Reader* rd);
int Reader_get_core_quantity(Reader* rd);
void* reader_thread(void* args);


Reader* Reader_create(int time_int, int core_quantity){
    Reader* rd = malloc(sizeof(*rd));
    *rd = (Reader){ .time_interv = time_int,
                    .core_quantity = core_quantity};
    return rd;
}

void Reader_destroy(Reader* rd){
    free(rd);
}


int Reader_get_time(Reader* rd){
    return rd->time_interv;
}

int Reader_get_core_quantity(Reader* rd){
    return rd->core_quantity;
}

void* reader_thread(void* args){
    Reader_Utils* utils = *(Reader_Utils**)args;
    Buffer* buffer = Reader_Utils_get_buffer(utils);
    Buffer* logger = Reader_Utils_get_logger(utils);
    Reader* reader = Reader_Utils_get_reader(utils);

    unsigned int time_interv = (unsigned int)Reader_get_time(reader);
    int lines_to_read = Reader_get_core_quantity(reader) + 1;
    
    FILE* file;
    Pack* pc = NULL;

    while(true) {
        file = fopen("/proc/stat","r");
        if(file == NULL){
            printf("ERROR WHILE OPENING A FILE!\n");
        }


        Buffer_lock(logger);
        if(Buffer_is_full(logger)){
            Buffer_wait_for_consumer(logger);
        }
        Pack* pc = Pack_create("[READER]SUCCESSFULLY OPENED FILE");
        Buffer_put(logger,pc);
        Buffer_call_consumer(logger);
        Buffer_unlock(logger);


        Buffer_lock(buffer);
        for(int i = 0; i < lines_to_read; ++i){
            if (Buffer_is_full(buffer)){
                Buffer_wait_for_consumer(buffer);
            } else {
                char line[MAX_SIZE];
                if(fgets(line,MAX_SIZE,file) == NULL) {
                    perror("ERROR WHILE GETTING LINE FROM FILE");
                }
                pc = Pack_create(line);
                Buffer_put(buffer,pc);
                Pack_destroy(pc);
                pc = NULL;
            }
        }
        Buffer_call_consumer(buffer);
        Buffer_unlock(buffer);
        Buffer_lock(logger);
        if(Buffer_is_full(logger)){
            Buffer_wait_for_consumer(logger);
        }
        Pack* pc = Pack_create("[READER]SENT PACKAGE");
        Buffer_put(logger,pc);
        Buffer_call_consumer(logger);
        Buffer_unlock(logger);
        fclose(file);
        usleep(time_interv);
    }
    return NULL;
}
