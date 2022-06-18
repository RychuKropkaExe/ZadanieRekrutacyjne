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

    
    Buffer* Buffer_create(size_t buffer_size);
    void Buffer_destroy(Buffer* bf);
    bool Buffer_is_full(const Buffer* bf);
    bool Buffer_is_empty(const Buffer* bf);
    void Buffer_put(Buffer* bf, Pack* product);
    char* Buffer_get(Buffer* bf);
    void Buffer_lock(Buffer* bf);
    void Buffer_unlock(Buffer* bf);
    void Buffer_call_producer(Buffer* bf);
    void Buffer_call_consumer(Buffer* bf);
    void Buffer_wait_for_producer(Buffer* bf);
    void Buffer_wait_for_consumer(Buffer* bf);


    Pack* Pack_create(char content[]);
    char* Pack_get_content(Pack* pc);
    void Pack_destroy(Pack* pc);


    Results_buffer* Results_buffer_create(size_t buffer_size);
    void Results_buffer_destroy(Results_buffer* bf);
    bool Results_buffer_is_full(const Results_buffer* bf);
    bool Results_buffer_is_empty(const Results_buffer* bf);
    void Results_buffer_put(Results_buffer* bf, double value);
    double Results_buffer_get(Results_buffer* bf);
    void Results_buffer_lock(Results_buffer* bf);
    void Results_buffer_unlock(Results_buffer* bf);
    void Results_buffer_call_producer(Results_buffer* bf);
    void Results_buffer_call_consumer(Results_buffer* bf);
    void Results_buffer_wait_for_producer(Results_buffer* bf);
    void Results_buffer_wait_for_consumer(Results_buffer* bf);
#endif

