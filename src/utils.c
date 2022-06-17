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

//Utils przechowuje pointery na używane struct pointery, tak by można było bezpiecznie przekazać kilka argumentów do wątków

typedef struct Reader_Utils{
    Buffer** bf;
    Reader** rd;
} Reader_Utils;

Reader_Utils* Reader_Utils_create(Buffer** bf,Reader** rd);
void Reader_Utils_destroy(Reader_Utils* utils);
Buffer* Reader_Utils_get_buffer(Reader_Utils* utils);
Reader* Reader_Utils_get_reader(Reader_Utils* utils);

Buffer* Reader_Utils_get_buffer(Reader_Utils* utils){
    return *(utils->bf);
}

Reader* Reader_Utils_get_reader(Reader_Utils* utils){
        return *(utils->rd);
}

Reader_Utils* Reader_Utils_create(Buffer** bf,Reader** rd){
    Reader_Utils* ut = malloc(sizeof(*ut));
    *ut = (Reader_Utils){  .bf = bf,
                    .rd = rd};
    return ut;
}

void Reader_Utils_destroy(Reader_Utils* utils){
    free(utils);
}

typedef struct Analyzer_Utils{
    Buffer** bf;
    Analyzer** al;
} Analyzer_Utils;

Analyzer_Utils* Analyzer_Utils_create(Buffer** bf, Analyzer** al){
    Analyzer_Utils* al = malloc(sizeof(*al));
    *al = (Analyzer_Utils){ .bf = bf,
                            .al = al
    };
    return al;
}