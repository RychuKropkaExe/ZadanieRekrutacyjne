#ifndef BUFFER_TESTS_H
#define BUFFER_TESTS_H
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include "../headers/buffer.h"

    void buffers_tests(void);
    static void buffer_test(void);
    static void results_buffer_test(void);
    static void pack_test(void);

    void buffers_tests(void){
        buffer_test();
        results_buffer_test();
        pack_test();
    }

    static void buffer_test(void){
        Buffer* buffer = Buffer_create(1);
        assert(buffer != NULL);
        char* test = "THIS IS A TEST STRING";
        Pack* pc = NULL; 

        pc = Pack_create(test);
        assert(pc != NULL);
        Buffer_put(buffer,pc);
        assert(Buffer_is_empty(buffer) != true);
        assert(Buffer_is_full(buffer) != true);
        Buffer_put(buffer,pc);
        Buffer_put(buffer,pc);
        Buffer_put(buffer,pc);
        Buffer_put(buffer,pc);
        Buffer_put(buffer,pc);
        assert(Buffer_is_full(buffer) == true);
        Pack_destroy(pc);
        pc = Buffer_get(buffer);
        char* result = Pack_get_content(pc);
        assert(strlen(test) == strlen(result));
        assert(strcmp(test,result) == 0);

        Buffer_destroy(buffer);
        Pack_destroy(pc);
        printf("PASSED BUFFER TEST!\n");
    }

    static void results_buffer_test(void){
        Results_buffer* buffer = Results_buffer_create(1);
        assert(buffer!=NULL);
        double test = 20;
        Results_buffer_put(buffer,test);
        assert(Results_buffer_is_empty(buffer) != true);
        assert(Results_buffer_is_full(buffer) != true);
        Results_buffer_put(buffer,test);
        Results_buffer_put(buffer,test);
        Results_buffer_put(buffer,test);
        Results_buffer_put(buffer,test);
        Results_buffer_put(buffer,test);
        assert(Results_buffer_is_full(buffer) == true);
        double result = Results_buffer_get(buffer);
        assert(test==result);
        Results_buffer_destroy(buffer);
        printf("PASSED RESULTS_BUFFER TEST!\n");
    }

    void pack_test(void){
        char* test = "PACK TEST!";
        Pack* pc = Pack_create(test);
        assert(pc != NULL);
        char* result = Pack_get_content(pc);
        assert(strcmp(test,result) == 0);
        Pack_destroy(pc);
        printf("PASSED PACK TEST!\n");
    }
#endif
