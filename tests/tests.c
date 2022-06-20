#include <stdio.h>
#include "buffer_tests.h"
#include "watchdog_tests.h"
#include "reader_tests.h"
#include "utils_tests.h"

int main(void){
    buffers_tests();
    watchdog_tests();
    reader_test();
    utils_test();
    return 0;
}
