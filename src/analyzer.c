#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../headers/analyzer.h"
#include "../headers/buffer.h"
#include "../headers/utils.h"


//przechowuje czasy z danych od readera
typedef struct Analyzer{
    double user;
    double nice;
    double system;
    double idle;
    double iowait;
    double irq;
    double softirq;
    double steal;
    double guest;
    double guest_nice;
    int core_quantity;
    char pad[4];
} Analyzer;

Analyzer* Analyzer_create(int core_quantity);
int Analyzer_get_core_quantity(Analyzer* al);
void* analyzer_thread(void* args);
void Analyzer_destroy(Analyzer* al);

Analyzer* Analyzer_create(int core_quantity){
    Analyzer* al = malloc(sizeof(*al));
    *al = (Analyzer){   .core_quantity = core_quantity};
    return al;
}

int Analyzer_get_core_quantity(Analyzer* al){
    return al->core_quantity;
}

void Analyzer_destroy(Analyzer* al){
    free(al);
}

void* analyzer_thread(void* args){

    Analyzer_Utils* ut = *(Analyzer_Utils**)args;

    Buffer* buffer = Analyzer_Utils_get_buffer(ut);
    Buffer* logger = Analyzer_Utils_get_logger(ut);

    Analyzer* analyzer = Analyzer_Utils_get_analyzer(ut);

    Results_buffer* results_buffer = Analyzer_Utils_get_Results_buffer(ut);

    Dog* dog = Analyzer_Utils_get_dog(ut);

    int core_quantity = (int)Analyzer_get_core_quantity(analyzer);

    Local_storage* storage = Local_storage_create((size_t)core_quantity+1);

    //Tablice do przechowywania poprdzeniego pomiaru
    double prev_total[core_quantity+1];
    memset(prev_total,0,sizeof(double)*((size_t)core_quantity+1));
    double prev_idle[core_quantity+1];
    memset(prev_idle,0,sizeof(double)*((size_t)core_quantity+1));

    double cpu_usages[core_quantity+1];
    memset(cpu_usages,0,sizeof(double)*((size_t)core_quantity+1));

    //prosta flaga, by nie wysyłać danych z pierwszego pomiaru(potrzebujemy różnicy, więc dwóch pomiarów)
    bool enough_data = false;  

    Pack* pc = NULL;
    char* buffer_get = NULL;
    Log_message(logger,"[ANALYZER][INFO] STARTING THREAD\n");
    while(Dog_get_flag(dog)){
        Buffer_lock(buffer);
        if (Buffer_is_empty(buffer) && Dog_get_flag(dog)){
                Buffer_wait_for_producer(buffer);
        }  
        if(!Dog_get_flag(dog)){
            Buffer_unlock(buffer);
            break;
        }
        for(int i = 0; i < core_quantity + 1; ++i){ 
            if (Buffer_is_empty(buffer)){
                exit_on_error("[ANALYZER][ERROR] UNCOMPLETE PACKAGE!\n");
            }
            pc = Buffer_get(buffer);
            Local_storage_put(storage,pc);
            free(pc);
            pc = NULL;
        }
        Buffer_call_producer(buffer);
        Buffer_unlock(buffer);
        Log_message(logger,"[ANALYZER][INFO] SUCCESFULLY TOOK DATA\n");

        for(int i = 0; i < core_quantity + 1; ++i){
            char cpu_name[16];
            (void)cpu_name;
            char line[256];

            pc = Local_storage_get(storage);
            buffer_get = Pack_get_content(pc);
            memcpy(&line[0],buffer_get,256);
            sscanf( line,
                    "%s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", cpu_name,
                    &(analyzer->user),
                    &(analyzer->nice),
                    &(analyzer->system),
                    &(analyzer->idle),
                    &(analyzer->iowait),
                    &(analyzer->irq),
                    &(analyzer->softirq),
                    &(analyzer->steal),
                    &(analyzer->guest),
                    &(analyzer->guest_nice)
            );
            double Idle = analyzer->idle + analyzer->iowait;
            double NonIdle = analyzer->user + analyzer->nice + analyzer->system + analyzer->irq + analyzer->softirq + analyzer->steal;
            double total = Idle + NonIdle;
            if(enough_data) {
                double totald =  total - prev_total[i];
                double idled = Idle - prev_idle[i];
                double delta = totald - idled;
                cpu_usages[i] =  ((double)delta/(double)totald) * 100;
            }
            prev_total[i] = total;
            prev_idle[i] = Idle;  
            free(pc);
            pc = NULL;        
        }
        enough_data = true;
        Log_message(logger,"[ANALYZER][INFO] SUCCESFULLY PROCESSED DATA\n");
        Log_message(logger,"[ANALYZER][INFO] SENDING DATA TO PRINTER\n");
        Results_buffer_lock(results_buffer);
        if(Results_buffer_is_full(results_buffer) && Dog_get_flag(dog)){
            Results_buffer_wait_for_consumer(results_buffer);
        }
        if(!Dog_get_flag(dog)){
            break;
        }
        for(int i = 0; i < core_quantity + 1; ++i) {
            if(Results_buffer_is_full(results_buffer)){
                exit_on_error("[ANALYZER][ERROR] NOT WHOLE TRANSPORT TAKEN!\n");
                Results_buffer_unlock(results_buffer);
                break;
            }
            Results_buffer_put(results_buffer, cpu_usages[i]);
        }
        Results_buffer_call_consumer(results_buffer);
        Results_buffer_unlock(results_buffer);
        Dog_kick(dog);
        Log_message(logger,"[ANALYZER][INFO] SUCCESFULLY SENT DATA TO PRITNER\n");
    }
    Local_storage_destroy(storage);
    return NULL;
}

