#include <stdio.h>
#include "../headers/buffer.h"
#include <string.h>

int main(){
    size_t buffer_size = (sysconf(_SC_NPROCESSORS_ONLN) + 1)*6;
    Buffer* bf = Buffer_create(buffer_size);
    char s[250] = {0};
    for(int i =0; i < 249; ++i) 
        s[i] = 's';
    Pack* pc = Pack_create(s);
    Buffer_put(bf,pc);
    Pack_destroy(pc);
    char* get = Buffer_get(bf);
    printf("%s\n",get);
    printf("%d\n",get[249]);
    printf("%zu\n", strlen(get));
    printf("%s\n",s);
    printf("%d\n",s[249]);
    printf("%zu\n", strlen(s));
    Buffer_destroy(bf);
    free(get);
    return 0;
}