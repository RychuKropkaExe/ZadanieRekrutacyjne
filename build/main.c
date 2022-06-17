#include <stdio.h>
#include "../headers/buffer.h"

int main(){
    long core_number = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%ld\n", core_number);
    Buffer* bf = Buffer_create(core_number);
    Pack* pc = Pack_create("SIEMA");
    Buffer_put(bf,pc);
    Pack_destroy(pc);
    char* get = Buffer_get(bf);
    printf("%s\n",get);
    Buffer_destroy(bf);
    free(get);
    return 0;
}