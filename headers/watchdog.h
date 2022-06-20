#ifndef WATCHDOG_G
#define WATCHDOG_G
    typedef struct Watchdog Watchdog;
    typedef struct Dog Dog;

    void exit_on_error(char* error);

    Watchdog* Watchdog_create();
    Dog* Watchdog_get_reader(Watchdog* wd);
    Dog* Watchdog_get_analyzer(Watchdog* wd);
    Dog* Watchdog_get_printer(Watchdog* wd);
    Dog* Watchdog_get_logger(Watchdog* wd);
    void Watchdog_destroy(Watchdog* wd);
    void Watchdog_alarm(Watchdog* wt);


    bool Dog_get_flag(Dog* dog);
    int Dog_attack(Dog* dog);
    void Dog_lock(Dog* dog);
    void Dog_unlock(Dog* dog);
    void Dog_set_flag(Dog* dog);
    void Dog_kick(Dog* dog);

    void* watchdog_thread(void* args);

#endif