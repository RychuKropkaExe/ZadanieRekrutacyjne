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
#include "../headers/watchdog.h"
//Maksymalna ilość bajtów w paczce, jedna linia z /proc/stat nie przekroczy tej wartości
//A taki rozmiar, jest raczej bardzo pesymistycznym górnym ograniczeniem
#define MAX_SIZE 256

typedef struct Reader{
    int time_interv;
    int core_quantity;
} Reader;


Reader* Reader_create(const int time_int, const int core_quantity){
    Reader* rd = malloc(sizeof(*rd));
    if(rd == NULL)
        exit_on_error("MALLOC FAILED!\n");
    *rd = (Reader){ .time_interv = time_int,
                    .core_quantity = core_quantity};
    return rd;
}

void Reader_destroy(Reader* const rd){
    free(rd);
}


int Reader_get_time(Reader* const rd){
    return rd->time_interv;
}

int Reader_get_core_quantity(Reader* const rd){
    return rd->core_quantity;
}

void* reader_thread(void* const args){
    Reader_Utils* utils = *(Reader_Utils**)args;
    Buffer* buffer = Reader_Utils_get_buffer(utils);
    Buffer* logger = Reader_Utils_get_logger(utils);
    Reader* reader = Reader_Utils_get_reader(utils);
    Dog* dog = Reader_Utils_get_dog(utils);

    unsigned int time_interv = (unsigned int)Reader_get_time(reader);

    int lines_to_read = Reader_get_core_quantity(reader) + 1;
    
    FILE* file;
    Pack* pc = NULL;

    Local_storage* storage = Local_storage_create((size_t)lines_to_read);

    Log_message(logger,"[READER][INFO] STARTING THREAD\n");

    while(Dog_get_flag(dog)){
        file = fopen("/proc/stat","r");
        if(file == NULL){
            Log_message(logger,"[READER][ERROR] COULD NOT OPEN A FILE!\n");
            exit_on_error("[READER][ERROR] COULD NOT OPEN A FILE\n");
            break;
        }
        Log_message(logger,"[READER][INFO] SUCCESSFULLY OPENED FILE\n");
        for(int i = 0; i < lines_to_read; ++i){
            char line[MAX_SIZE];
            if(fgets(line,MAX_SIZE,file) == NULL) {
                Log_message(logger,"[READER][ERROR] COULD NOT GET LINE FROM FILE\n");
            }
            pc = Pack_create(line);
            Local_storage_put(storage,pc);
            Pack_destroy(pc);
            pc = NULL;
        }

        if(!Dog_get_flag(dog)){
            fclose(file);
            break;
        }

        Buffer_lock(buffer);
        if (Buffer_is_full(buffer) && Dog_get_flag(dog)){
            Buffer_wait_for_consumer(buffer);
        }
        if(!Dog_get_flag(dog)){
            Buffer_unlock(buffer);
            fclose(file);
            break;
        }
        for(int i = 0; i < lines_to_read; ++i){
            if (Buffer_is_full(buffer)){
                exit_on_error("[READE][ERROR] NOT WHOLE TRANSPORT WAAS TAKEN\n");
                break;
            }
            pc = Local_storage_get(storage);
            Buffer_put(buffer,pc);
            Pack_destroy(pc);
        }
        Buffer_call_consumer(buffer);
        Buffer_unlock(buffer);
        Log_message(logger,"[READER][INFO] SENT PACKAGE\n");
        Dog_kick(dog);
        fclose(file);
        if(Dog_get_flag(dog))
            usleep(time_interv);
    }
    Local_storage_destroy(storage);
    Log_message(logger,"[READER][INFO] READER EXITED SUCCESSFUFLY!\n");
    return NULL;
}
