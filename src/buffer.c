#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../headers/buffer.h"

//Buffer przechowuje paczki wysyłane od readera do analyzera
// Zakładamy maksymalny rozmiar każdej z liczb z /proc/stat na 8 bajtów, czyli może mieć maksymalnie
// 20 znaków, dodając do tego string zawierający nazwe cpu, spacje itd zaokrąglamy do 250 charów
#define PACK_SIZE 250

//Wrapper na paczke od analyzera do printera
typedef struct Pack
{
    char pack[PACK_SIZE];
} Pack;
//Buffer współdzielony pomiędzy readerem i Analyzerem, FAM jest po to, że urządzenie może mieć różną ilość rdzeni
//a chcielibyśmy móc przechowywać określoną ilość Paczek(docelowo pewną wielokrotność jedengo transportu) w bufferze
typedef struct Buffer
{   
    size_t size;
    size_t head;
    size_t tail;
    size_t max_size;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    Pack buffer[]; //FAM

} Buffer;

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

Buffer* Buffer_create(size_t buffer_size)
{

    Buffer* buffer = malloc(sizeof(*buffer) + sizeof(Pack)*buffer_size);
    if (buffer == NULL)
        return NULL;
    buffer->size = 0;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->max_size = buffer_size;
    pthread_mutex_init(&(buffer->mutex),NULL);
    pthread_cond_init(&(buffer->can_consume), NULL);
    pthread_cond_init(&(buffer->can_produce), NULL);
    return buffer;
}

void Buffer_destroy(Buffer* bf)
{
    pthread_cond_destroy(&bf->can_consume);
    pthread_cond_destroy(&bf->can_produce);
    pthread_mutex_destroy(&bf->mutex);

    free(bf);
}

bool Buffer_is_full(const Buffer* bf)
{
    return bf->size == bf->max_size;
}

bool Buffer_is_empty(const Buffer* bf)
{
    return bf->size == 0;
}

void Buffer_put(Buffer* bf, Pack* pack)
{
    bf->buffer[bf->head] = *pack;
    bf->head = (bf->head + 1) % bf->max_size;
    ++bf->size;
}

char* Buffer_get(Buffer* bf)
{
    Pack* pack = malloc(sizeof(Pack));
    *pack = bf->buffer[bf->tail];
    bf->tail = (bf->tail + 1) % bf->max_size;
    --bf->size;

    return Pack_get_content(pack);
}

void Buffer_lock(Buffer* bf)
{
    pthread_mutex_lock(&bf->mutex);
}

void Buffer_unlock(Buffer* bf)
{
    pthread_mutex_unlock(&bf->mutex);
}

void Buffer_call_producer(Buffer* bf)
{
    pthread_cond_signal(&bf->can_produce);
}

void Buffer_call_consumer(Buffer* bf)
{
    pthread_cond_signal(&bf->can_consume);
}

void Buffer_wait_for_producer(Buffer* bf)
{
    pthread_cond_wait(&bf->can_consume, &bf->mutex);
}

void Buffer_wait_for_consumer(Buffer* bf)
{
    pthread_cond_wait(&bf->can_produce, &bf->mutex);
}

Pack* Pack_create(char content[]){
    Pack* pc = malloc(sizeof(*pc));
    if (pc == NULL)
        return NULL;
    *pc = (Pack){.pack = {0}};
    memcpy(&(pc->pack[0]),content,strlen(content)+1);
    return pc;
}

void Pack_destroy(Pack* pc)
{
    free(pc);
}

char* Pack_get_content(Pack* pc){
    return pc->pack;
}

