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

#define MAX_STAGNATION 1


void set_flag(void);
bool get_flag(void);


static bool flag = true;
static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_mutex_t mutex;

void exit_gracefully(const int signum){
    printf("SIGTERM CAUGHT %d\n", signum);
    set_flag();
}

void exit_on_error(const char error[static 1]){
    printf("%s", error);
    set_flag();
}

static void init(void) {    
    pthread_mutex_init(&mutex, NULL);
}

void set_flag(void){
    pthread_once(&once,init);
    pthread_mutex_lock(&mutex);
    flag = false;
    pthread_mutex_unlock(&mutex);
}

bool get_flag(void){
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
    char pad[6];
    bool flag;
    bool is_alive;
} Dog;




Watchdog* Watchdog_create(void){
    Watchdog* watchdog = malloc(sizeof(*watchdog));

    Dog* reader = malloc(sizeof(*reader));
    if(reader == NULL)
        exit_on_error("MALLOC FAILED!\n");
    pthread_mutex_init(&(reader->mutex),NULL);
    reader->flag = true;
    reader->is_alive = true;
    watchdog->reader=reader;

    Dog* analyzer = malloc(sizeof(*analyzer));
    if(analyzer == NULL)
        exit_on_error("MALLOC FAILED!\n");
    pthread_mutex_init(&(analyzer->mutex),NULL);
    analyzer->flag = true;
    analyzer->is_alive = true;
    watchdog->analyzer=analyzer;

    Dog* printer = malloc(sizeof(*printer));
    if(printer == NULL)
        exit_on_error("MALLOC FAILED!\n");
    pthread_mutex_init(&(printer->mutex),NULL);
    printer->flag = true;
    printer->is_alive = true;
    watchdog->printer=printer;

    Dog* logger = malloc(sizeof(*logger));
    if(logger == NULL)
        exit_on_error("MALLOC FAILED!\n");
    pthread_mutex_init(&(logger->mutex),NULL);
    logger->flag = true;
    logger->is_alive = true;
    watchdog->logger=logger;

    return watchdog;
}

Dog* Watchdog_get_reader(Watchdog* const wd){
    return wd->reader;
}

Dog* Watchdog_get_analyzer(Watchdog* const wd){
    return  wd->analyzer;
}

Dog* Watchdog_get_printer(Watchdog* const wd){
    return wd->printer;
}

Dog* Watchdog_get_logger(Watchdog* const wd){
    return wd->logger;
}



void Watchdog_destroy(Watchdog* const wd){
    free(wd->reader);
    free(wd->analyzer);
    free(wd->printer);
    free(wd->logger);
    free(wd);
}

void Dog_lock(Dog* const dog){
    pthread_mutex_lock(&(dog->mutex));
}

void Dog_unlock(Dog* const dog){
    pthread_mutex_unlock(&(dog->mutex));
}

void Dog_kick(Dog* const dog){
    dog->is_alive = true;
}

int Dog_attack(Dog* const dog){
    if(dog->is_alive == true){
        dog->is_alive = false;
        return 0;
    } else {
        return 1;
    }
}

bool Dog_get_flag(Dog* const dog){
    bool f = true;
    Dog_lock(dog);
    f = dog->flag;
    Dog_unlock(dog);
    return f;
}

void Dog_set_flag(Dog* const dog){
    Dog_lock(dog);
    dog->flag = false;
    Dog_unlock(dog);
}

void Watchdog_alarm(Watchdog* const wt){
    Dog_set_flag(wt->reader);
    Dog_set_flag(wt->analyzer);
    Dog_set_flag(wt->printer);
    Dog_set_flag(wt->logger);
}

void* watchdog_thread(void* const args){
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

    bool flip = 1;
    while(get_flag()){
        if(flip){
            if (Dog_attack(reader_dog)){
                printf("READER STAGNATION! \n");
                Watchdog_alarm(pack);
                break;
            }
            if(Dog_attack(analyzer_dog)){
                printf("ANALYZER STAGNATION! \n");
                Watchdog_alarm(pack);
                break;
            }
            if(Dog_attack(printer_dog)){
                printf("PRINTER STAGNATION!\n");
                Watchdog_alarm(pack);
                break;
            }
            if(Dog_attack(logger_dog)){
                printf("LOGGER STAGNATION! \n");
                Watchdog_alarm(pack);
                break;
            }
        }
        flip = !flip;
        sleep(MAX_STAGNATION);
    }
    Watchdog_alarm(pack);
    Buffer_call_consumer(buffer);
    Buffer_call_consumer(logger);
    Buffer_call_producer(buffer);
    Buffer_call_producer(logger);
    Results_buffer_call_consumer(results_buffer);
    Results_buffer_call_producer(results_buffer);
    return NULL;
}

