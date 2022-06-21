#ifndef READER_H
#define READER_H
    typedef struct Reader Reader;
    Reader* Reader_create(int time_int, int core_quantity);
    void Reader_destroy(Reader* const rd);
    int Reader_get_time(Reader* const rd);
    int Reader_get_core_quantity(Reader* const rd);
    void* reader_thread(void* const args);
#endif
