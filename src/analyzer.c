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
} Analyzer;

Analyzer* Analyzer_create(int core_quantity);
int Analyzer_get_core_quantity(Analyzer* al);
void analyzer_thread(void* args);

Analyzer* Analyzer_create(int core_quantity){
    Analyzer* al = malloc(sizeof(*al));
    *al = (Analyzer){   .core_quantity = core_quantity};
    return al;
}

int Analyzer_get_core_quantity(Analyzer* al){
    return al->core_quantity;
}

void analyzer_thread(void* args){
    //Analyzer_Utils* ut = *(Analyzer_Utils**)args;
    while(true){
        
    }
}