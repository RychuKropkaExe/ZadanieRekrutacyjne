#ifndef WATCHDOG_TESTS
#define WATCHDOG_TESTS
    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include "../headers/watchdog.h"

    void watchdog_tests(void);
    static void watchdog_test(void);


    void watchdog_tests(void){
        watchdog_test();
    }
    
    static void watchdog_test(void){
        Watchdog* watchdog = Watchdog_create();
        assert(watchdog != NULL);
        Dog* reader_dog = Watchdog_get_reader(watchdog);
        Dog* analyzer_dog = Watchdog_get_analyzer(watchdog);
        Dog* printer_dog = Watchdog_get_printer(watchdog);
        Dog* logger_dog = Watchdog_get_logger(watchdog);
        assert(reader_dog != NULL);
        assert(analyzer_dog != NULL);
        assert(printer_dog != NULL);
        assert(logger_dog != NULL);
        assert(Dog_get_flag(reader_dog) == true);
        assert(Dog_get_flag(analyzer_dog) == true);
        assert(Dog_get_flag(printer_dog) == true);
        assert(Dog_get_flag(logger_dog) == true);
        Watchdog_alarm(watchdog);
        assert(Dog_get_flag(reader_dog) == false);
        assert(Dog_get_flag(analyzer_dog) == false);
        assert(Dog_get_flag(printer_dog) == false);
        assert(Dog_get_flag(logger_dog) == false);
        printf("PASSED WATCHDOG TESTS!\n");
        Watchdog_destroy(watchdog);
    }
#endif
