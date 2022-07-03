#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#ifndef BUFFER_H
#define BUFFER_H
    typedef struct Pack Pack;
    typedef struct Buffer Buffer;
    typedef struct Results_buffer Results_buffer;
    typedef struct Local_storage Local_storage;
    
    Buffer* Buffer_create(size_t buffer_size);
    void Buffer_destroy(Buffer* const bf);
    bool Buffer_is_full(const Buffer* bf);
    bool Buffer_is_empty(const Buffer* bf);
    void Buffer_put(Buffer* const bf, const Pack* const product);
    Pack* Buffer_get(Buffer* const bf);
    void Buffer_lock(Buffer* const bf);
    void Buffer_unlock(Buffer* const bf);
    void Buffer_call_producer(Buffer* const bf);
    void Buffer_call_consumer(Buffer* const bf);
    void Buffer_wait_for_producer(Buffer* const bf);
    void Buffer_wait_for_consumer(Buffer* const bf);


    Pack* Pack_create(const char content[]);
    char* Pack_get_content(Pack* const pc);
    void Pack_destroy(Pack* const pc);


    Results_buffer* Results_buffer_create(const size_t buffer_size);
    void Results_buffer_destroy(Results_buffer* const bf);
    bool Results_buffer_is_full(const Results_buffer* const bf);
    bool Results_buffer_is_empty(const Results_buffer* const bf);
    void Results_buffer_put(Results_buffer* const bf, const double value);
    double Results_buffer_get(Results_buffer* const bf);
    void Results_buffer_lock(Results_buffer* const bf);
    void Results_buffer_unlock(Results_buffer* const bf);
    void Results_buffer_call_producer(Results_buffer* const bf);
    void Results_buffer_call_consumer(Results_buffer* const bf);
    void Results_buffer_wait_for_producer(Results_buffer* const bf);
    void Results_buffer_wait_for_consumer(Results_buffer* const bf);

    Local_storage* Local_storage_create(const size_t size);
    void Local_storage_put(Local_storage* const ls, const Pack* const pack);
    Pack* Local_storage_get(Local_storage* const ls);
    void Local_storage_destroy(Local_storage* const ls);

    int Log_message(Buffer* logger, const char msg[static 1]);
    
#endif

