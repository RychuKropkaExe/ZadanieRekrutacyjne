#ifndef WATCHDOG_G
#define WATCHDOG_G
    #include <stdbool.h>
    
    typedef struct Watchdog Watchdog;
    typedef struct Dog Dog;

    void exit_on_error(const char error[static 1]);
    void exit_gracefully(int signum);

    Watchdog* Watchdog_create(void);
    Dog* Watchdog_get_reader(Watchdog* const wd);
    Dog* Watchdog_get_analyzer(Watchdog* const wd);
    Dog* Watchdog_get_printer(Watchdog* const wd);
    Dog* Watchdog_get_logger(Watchdog* const wd);
    void Watchdog_destroy(Watchdog* const wd);
    void Watchdog_alarm(Watchdog* const wt);


    bool Dog_get_flag(Dog* const dog);
    int Dog_attack(Dog* const dog);
    void Dog_lock(Dog* const dog);
    void Dog_unlock(Dog* const dog);
    void Dog_set_flag(Dog* const dog);
    void Dog_kick(Dog* const dog);

    void* watchdog_thread(void* const args);

#endif

