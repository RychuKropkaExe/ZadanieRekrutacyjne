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
#include "../headers/watchdog.h"

#define MAX_STAGNATION 2
void set_flag();
bool get_flag();
void exit_on_error(char* error);

static bool flag = true;

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_mutex_t mutex;

void exit_gracefully(int signum){
    printf("SIGTERM CAUGHT\n");
    set_flag();
}

void exit_on_error(char* error){
    printf("%s", error);
    set_flag();
}

void init() {    
    pthread_mutex_init(&mutex, NULL);
}

void set_flag(){
    pthread_once(&once,init);
    pthread_mutex_lock(&mutex);
    flag = false;
    pthread_mutex_unlock(&mutex);
}

bool get_flag(){
    bool f = false;
    pthread_once(&once,init);
    pthread_mutex_lock(&mutex);
    f = flag;
    pthread_mutex_unlock(&mutex);
    return f;
}


typedef struct Watchdog{
    Dog* reader;
    Dog* analyzer;
    Dog* printer;
    Dog* logger;
} Watchdog;

typedef struct Dog{
    pthread_mutex_t mutex;
    bool flag;
    bool is_alive;
} Dog;

Watchdog* Watchdog_create();
Dog* Watchdog_get_reader(Watchdog* wd);
Dog* Watchdog_get_analyzer(Watchdog* wd);
Dog* Watchdog_get_printer(Watchdog* wd);
Dog* Watchdog_get_logger(Watchdog* wd);
void Watchdog_destroy(Watchdog* wd);
void Watchdog_alarm(Watchdog* wt);


bool Dog_get_flag(Dog* dog);
int Dog_attack(Dog* dog);
void Dog_lock(Dog* dog);
void Dog_unlock(Dog* dog);
void Dog_set_flag(Dog* dog);
void Dog_kick(Dog* dog);

void* watchdog_thread(void* args);



Watchdog* Watchdog_create(){
    Watchdog* watchdog = malloc(sizeof(*watchdog));

    Dog* reader = malloc(sizeof(*reader));
    pthread_mutex_init(&(reader->mutex),NULL);
    reader->flag = 1;
    reader->is_alive = 1;
    watchdog->reader=reader;

    Dog* analyzer = malloc(sizeof(*analyzer));
    pthread_mutex_init(&(analyzer->mutex),NULL);
    analyzer->flag = true;
    analyzer->is_alive = true;
    watchdog->analyzer=analyzer;

    Dog* printer = malloc(sizeof(*printer));
    pthread_mutex_init(&(printer->mutex),NULL);
    printer->flag = 1;
    printer->is_alive = 1;
    watchdog->printer=printer;

    Dog* logger = malloc(sizeof(*logger));
    pthread_mutex_init(&(logger->mutex),NULL);
    logger->flag = 1;
    logger->is_alive = 1;
    watchdog->logger=logger;

    return watchdog;
}

Dog* Watchdog_get_reader(Watchdog* wd){
    return wd->reader;
}

Dog* Watchdog_get_analyzer(Watchdog* wd){
    return  wd->analyzer;
}

Dog* Watchdog_get_printer(Watchdog* wd){
    return wd->printer;
}

Dog* Watchdog_get_logger(Watchdog* wd){
    return wd->logger;
}



void Watchdog_destroy(Watchdog* wd){
    free(wd->reader);
    free(wd->analyzer);
    free(wd->printer);
    free(wd->logger);
    free(wd);
}

void Dog_lock(Dog* dog){
    pthread_mutex_lock(&(dog->mutex));
}

void Dog_unlock(Dog* dog){
    pthread_mutex_unlock(&(dog->mutex));
}

void Dog_kick(Dog* dog){
    dog->is_alive = true;
}

int Dog_attack(Dog* dog){
    if(dog->is_alive == true){
        dog->is_alive = false;
        return 0;
    } else {
        return 1;
    }



}

bool Dog_get_flag(Dog* dog){
    bool flag = true;
    Dog_lock(dog);
    flag = dog->flag;
    Dog_unlock(dog);
    return flag;
}

void Dog_set_flag(Dog* dog){
    Dog_lock(dog);
    dog->flag = false;
    Dog_unlock(dog);
}

void Watchdog_alarm(Watchdog* wt){
    Dog_set_flag(wt->reader);
    Dog_set_flag(wt->analyzer);
    Dog_set_flag(wt->printer);
    Dog_set_flag(wt->logger);
}

void* watchdog_thread(void* args){
    pthread_mutex_init(&mutex,NULL);
    Watchdog_Utils* utils = *(Watchdog_Utils**)args;
    Buffer* buffer = Watchdog_Utils_get_buffer(utils);
    Buffer* logger = Watchdog_Utils_get_logger(utils);
    Results_buffer* results_buffer = Watchdog_Utils_get_results_buffer(utils);
    Watchdog* pack = Watchdog_Utils_get_watchdog(utils);
    Dog* reader_dog = Watchdog_get_reader(pack);
    Dog* analyzer_dog = Watchdog_get_analyzer(pack);
    Dog* printer_dog = Watchdog_get_printer(pack);
    Dog* logger_dog = Watchdog_get_logger(pack);
    Log_message(logger,"[WATCHDOG][INFO] STARTING THREAD!\n");
    while(get_flag()){
        if (Dog_attack(reader_dog)){
            Log_message(logger,"[WATCHDOG][ERROR] READER DIDNT RESPOND, ABORTING\n");
            Watchdog_alarm(pack);
            break;
        }
        if(Dog_attack(analyzer_dog)){
            Log_message(logger,"[WATCHDOG][ERROR] ANALYZER DIDNT RESPOND, ABORTING\n");
            Watchdog_alarm(pack);
            break;
        }
        if(Dog_attack(printer_dog)){
            Log_message(logger,"[WATCHDOG][ERROR] PRINTER DIDNT RESPOND, ABORTING\n");
            Watchdog_alarm(pack);
            break;
        }
        if(Dog_attack(logger_dog)){
            printf("LOGGER DIDNT RESPOND, ABORTING!\n");
            Watchdog_alarm(pack);
            break;
        }
        sleep(MAX_STAGNATION);
    }
    Buffer_call_consumer(buffer);
    Buffer_call_consumer(logger);
    Buffer_call_producer(buffer);
    Buffer_call_producer(logger);
    Results_buffer_call_consumer(results_buffer);
    Results_buffer_call_producer(results_buffer);
    return NULL;
}

