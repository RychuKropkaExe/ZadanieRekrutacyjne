#include <stdio.h>
#include <string.h>
#include "../headers/buffer.h"
#include "../headers/reader.h"
#include "../headers/utils.h"

int main(){
    int core_quantity = sysconf(_SC_NPROCESSORS_ONLN);
    size_t time_interv = 1;
    Buffer* bf = Buffer_create(core_quantity+1);
    Reader* rd = Reader_create(time_interv, core_quantity);
    Reader_Utils* ut = Reader_Utils_create(&bf,&rd);
    reader_thread(&ut);
    Buffer_destroy(bf);
    Reader_destroy(rd);
    Reader_Utils_destroy(ut);
    return 0;
}