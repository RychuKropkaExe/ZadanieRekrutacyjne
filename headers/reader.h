#ifndef READER_H
#define READER_H
    typedef struct Reader Reader;
    Reader* Reader_create(int time_int, int core_quantity);
    void Reader_destroy(Reader* rd);
    int Reader_get_time(Reader* rd);
    int Reader_get_core_quantity(Reader* rd);
    void* reader_thread(void* args);
#endif
