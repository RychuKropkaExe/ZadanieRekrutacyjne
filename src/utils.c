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

//Wrapery które są przekazywane do poszczególnych wątków

typedef struct Reader_Utils{
    Buffer* bf;
    Buffer* logger;
    Reader* rd;
} Reader_Utils;

typedef struct Analyzer_Utils{
    Buffer* bf;
    Buffer* logger;
    Analyzer* al;
    Results_buffer* rbf;
} Analyzer_Utils;

typedef struct Printer_Utils{
    Results_buffer* rbf;
    Buffer* logger;
    int core_quantity;
    char pad[4];
} Printer_Utils;


Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd);
Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
Buffer* Reader_Utils_get_logger(Reader_Utils* utils);
Reader* Reader_Utils_get_reader(Reader_Utils* utils);
void Reader_Utils_destroy(Reader_Utils* utils);

Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf);
Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* utils);
Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* utils);
Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* utils);
Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* utils);
void Analyzer_Utils_destroy(Analyzer_Utils* utils);


Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, int core_quantity);
Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils);
Buffer* Printer_Utils_get_logger(Printer_Utils* utils);
int Printer_Utils_get_core_quantity(Printer_Utils* utils);
void Printer_Utils_destroy(Printer_Utils* utils);


Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd){
    Reader_Utils* ut = malloc(sizeof(*ut));
    *ut = (Reader_Utils){   .bf = bf,
                            .logger = logger,
                            .rd = rd};
    return ut;
}

Buffer* Reader_Utils_get_buffer(Reader_Utils* utils){
    return utils->bf;
}

Buffer* Reader_Utils_get_logger(Reader_Utils* utils) {
    return utils->logger;
}

Reader* Reader_Utils_get_reader(Reader_Utils* utils){
        return utils->rd;
}

void Reader_Utils_destroy(Reader_Utils* utils){
    free(utils);
}


Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf){
    Analyzer_Utils* ut = malloc(sizeof(*ut));
    *ut = (Analyzer_Utils){ .bf = bf,
                            .logger = logger,
                            .al = al,
                            .rbf = rbf
    };
    return ut;
}

Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* utils){
    return utils->bf;
}

Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* utils){
    return utils->logger;
}

Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* utils){
    return utils->al;
}

Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* utils){
    return utils->rbf;
}

void Analyzer_Utils_destroy(Analyzer_Utils* utils){
    free(utils);
}


Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, int core_quantity) {
    Printer_Utils* utils = malloc(sizeof(*utils));
    *utils = (Printer_Utils){   .rbf = rbf,
                                .logger = logger,
                                .core_quantity = core_quantity};
    return utils;
}

Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils){
    return utils->rbf;
}

Buffer* Printer_Utils_get_logger(Printer_Utils* utils){
    return utils->logger;
}

void Printer_Utils_destroy(Printer_Utils* utils){
    free(utils);
}

int Printer_Utils_get_core_quantity(Printer_Utils* utils){
    return utils->core_quantity;
}

