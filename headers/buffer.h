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
#endif