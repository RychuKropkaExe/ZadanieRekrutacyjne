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


typedef struct Analyzer{
    int core_quantity;
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
    Analyzer* analyzer = Analyzer_Utils_get_analyzer(ut);
    Results_buffer* results_buffer = Analyzer_Utils_get_Results_buffer(ut);

    int core_quantity = Analyzer_get_core_quantity(analyzer);

    double prev_total[core_quantity+1];
    memset(prev_total,0,sizeof(double)*(core_quantity+1));
    double prev_idle[core_quantity+1];
    memset(prev_idle,0,sizeof(double)*(core_quantity+1));

    double cpu_usages[core_quantity+1];
    memset(cpu_usages,0,sizeof(double)*(core_quantity+1));

    bool enough_data = false;

    while(true){
        Buffer_lock(buffer);
        for(int i = 0; i < core_quantity + 1; ++i){
            if (Buffer_is_empty(buffer)){
                Buffer_wait_for_producer(buffer);
            }   

            char cpu_name[16];
            (void)cpu_name;
            char line[256];

            memcpy(&line[0],Buffer_get(buffer),256);
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
        }
        enough_data = true;
        Buffer_call_producer(buffer);
        Buffer_unlock(buffer);

        Results_buffer_lock(results_buffer);
        for(int i = 0; i < core_quantity + 1; ++i) {
            if(Results_buffer_is_full(results_buffer)){
                Results_buffer_wait_for_consumer(results_buffer);
            }
            Results_buffer_put(results_buffer, cpu_usages[i]);
        }
        Results_buffer_call_consumer(results_buffer);
        Results_buffer_unlock(results_buffer);
        for(int i = 0; i < core_quantity +1; ++i){
            printf("%lf\n", Results_buffer_get(results_buffer));
        }
    }
}