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
    int core_quantity = Analyzer_get_core_quantity(analyzer);
    int name_length = core_quantity/10 + 4;

    double prev_total[core_quantity+1];
    memset(prev_total,0,sizeof(double)*(core_quantity+1));
    double prev_idle[core_quantity+1];
    memset(prev_idle,0,sizeof(double)*(core_quantity+1));

    bool enough_data = false;

    while(true){
        Buffer_lock(buffer);
        for(int i = 0; i < core_quantity + 1; ++i){
            if (Buffer_is_empty(buffer)){
                Buffer_wait_for_producer(buffer);
            }
                char line[256];
                memcpy(&line[0],Buffer_get(buffer),256);
                //printf("%s",line);
                sscanf( &line[name_length],
                        "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
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
                    double CPU_Percentage =  ((double)delta/(double)totald) * 100;
                }
                prev_total[i] = total;
                prev_idle[i] = Idle;
            
        }
        enough_data = true;
        Buffer_call_producer(buffer);
        Buffer_unlock(buffer);
    }
}