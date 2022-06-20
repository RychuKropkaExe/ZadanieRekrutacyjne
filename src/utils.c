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


//Wrapery które są przekazywane do poszczególnych wątków
typedef struct Reader_Utils{
    Buffer* bf;
    Buffer* logger;
    Reader* rd;
    Dog* dog;
} Reader_Utils;

typedef struct Analyzer_Utils{
    Buffer* bf;
    Buffer* logger;
    Analyzer* al;
    Results_buffer* rbf;
    Dog* dog;
} Analyzer_Utils;

typedef struct Printer_Utils{
    Results_buffer* rbf;
    Buffer* logger;
    Dog* dog;
    int core_quantity;
} Printer_Utils;

typedef struct Logger_Utils{
    Buffer* logger;
    Dog* dog;
} Logger_Utils;

typedef struct Watchdog_Utils{
    Buffer* bf;
    Buffer* logger;
    Results_buffer* rbf;
    Watchdog* watchdog;
} Watchdog_Utils;

Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd, Dog* dog);
Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
Buffer* Reader_Utils_get_logger(Reader_Utils* utils);
Reader* Reader_Utils_get_reader(Reader_Utils* utils);
Dog* Reader_Utils_get_dog(Reader_Utils* utils);
void Reader_Utils_destroy(Reader_Utils* utils);

Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf, Dog* dog);
Buffer* Analyzer_Utils_get_buffer(Analyzer_Utils* utils);
Buffer* Analyzer_Utils_get_logger(Analyzer_Utils* utils);
Analyzer* Analyzer_Utils_get_analyzer(Analyzer_Utils* utils);
Dog* Analyzer_Utils_get_dog(Analyzer_Utils* utils);
Results_buffer* Analyzer_Utils_get_Results_buffer(Analyzer_Utils* utils);
void Analyzer_Utils_destroy(Analyzer_Utils* utils);


Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, Dog* dog,int core_quantity);
Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils);
Buffer* Printer_Utils_get_logger(Printer_Utils* utils);
Dog* Printer_Utils_get_dog(Printer_Utils* utils);
int Printer_Utils_get_core_quantity(Printer_Utils* utils);
void Printer_Utils_destroy(Printer_Utils* utils);


Logger_Utils* Logger_Utils_create(Buffer* logger, Dog* dog);
Buffer* Logger_Utils_get_logger(Logger_Utils* utils);
Dog* Logger_Utils_get_dog(Logger_Utils* utils);
void Logger_Utils_destroy(Logger_Utils* utils);


Watchdog_Utils* Watchdog_Utils_create(Watchdog* watchdog, Buffer* logger, Buffer* bf, Results_buffer* rbf);
Watchdog* Watchdog_Utils_get_watchdog(Watchdog_Utils* utils);
Buffer* Watchdog_Utils_get_buffer(Watchdog_Utils* utils);
Buffer* Watchdog_Utils_get_logger(Watchdog_Utils* utils);
Results_buffer* Watchdog_Utils_get_results_buffer(Watchdog_Utils* utils);
void Watchdog_Utils_destroy(Watchdog_Utils* utils);


Reader_Utils* Reader_Utils_create(Buffer* bf,Buffer* logger, Reader* rd, Dog* dog){
    Reader_Utils* ut = malloc(sizeof(*ut));
    *ut = (Reader_Utils){   .bf = bf,
                            .logger = logger,
                            .dog = dog,
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

Dog* Reader_Utils_get_dog(Reader_Utils* utils){
    return utils->dog;
}

void Reader_Utils_destroy(Reader_Utils* utils){
    free(utils);
}


Analyzer_Utils* Analyzer_Utils_create(Buffer* bf, Buffer* logger, Analyzer* al, Results_buffer* rbf, Dog* dog){
    Analyzer_Utils* ut = malloc(sizeof(*ut));
    *ut = (Analyzer_Utils){ .bf = bf,
                            .logger = logger,
                            .al = al,
                            .rbf = rbf,
                            .dog = dog
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

Dog* Analyzer_Utils_get_dog(Analyzer_Utils* utils){
    return utils->dog;
}

void Analyzer_Utils_destroy(Analyzer_Utils* utils){
    free(utils);
}


Printer_Utils*  Printer_Utils_create(Results_buffer* rbf,Buffer* logger, Dog* dog,int core_quantity) {
    Printer_Utils* utils = malloc(sizeof(*utils));
    *utils = (Printer_Utils){   .rbf = rbf,
                                .logger = logger,
                                .dog = dog,
                                .core_quantity = core_quantity};
    return utils;
}

Results_buffer* Printer_Utils_get_Results_buffer(Printer_Utils* utils){
    return utils->rbf;
}

Buffer* Printer_Utils_get_logger(Printer_Utils* utils){
    return utils->logger;
}

Dog* Printer_Utils_get_dog(Printer_Utils* utils){
    return utils->dog;
}

int Printer_Utils_get_core_quantity(Printer_Utils* utils){
    return utils->core_quantity;
}

void Printer_Utils_destroy(Printer_Utils* utils){
    free(utils);
}


Logger_Utils* Logger_Utils_create(Buffer* logger, Dog* dog){
    Logger_Utils* utils = malloc(sizeof(*utils));
    *utils = (Logger_Utils){    .logger = logger,
                                .dog = dog
    };
    return utils;
}

Buffer* Logger_Utils_get_logger(Logger_Utils* utils){
    return utils->logger;
}

Dog* Logger_Utils_get_dog(Logger_Utils* utils){
    return utils->dog;
}

void Logger_Utils_destroy(Logger_Utils* utils){
    free(utils);
}


Watchdog_Utils* Watchdog_Utils_create(Watchdog* watchdog, Buffer* logger, Buffer* bf, Results_buffer* rbf){
    Watchdog_Utils* utils = malloc(sizeof(*utils));
    *utils = (Watchdog_Utils){  .logger=logger,
                                .watchdog=watchdog,
                                .bf=bf,
                                .rbf=rbf};
    return utils;
}

Watchdog* Watchdog_Utils_get_watchdog(Watchdog_Utils* utils){
    return utils->watchdog;
}

Buffer* Watchdog_Utils_get_buffer(Watchdog_Utils* utils){
    return utils->bf;
}

Buffer* Watchdog_Utils_get_logger(Watchdog_Utils* utils){
    return utils->logger;
}

Results_buffer* Watchdog_Utils_get_results_buffer(Watchdog_Utils* utils){
    return utils->rbf;
}

void Watchdog_Utils_destroy(Watchdog_Utils* utils){
    free(utils);
}


