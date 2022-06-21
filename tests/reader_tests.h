#ifndef READER_TESTS_H
#define READER_TESTS_H
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include "../headers/reader.h"
    void reader_test(void);

    void reader_test(void){
        int test_core_quantity = 5;
        int test_time_int = 10;
        Reader* reader = Reader_create(test_time_int,test_core_quantity);
        assert(reader != NULL);
        int result_core_quantity = Reader_get_core_quantity(reader);
        int result_time_int = Reader_get_time(reader);
        assert(test_core_quantity == result_core_quantity);
        assert(test_time_int == result_time_int);
        printf("PASSED READER TEST!\n");
        Reader_destroy(reader);
    }
#endif
