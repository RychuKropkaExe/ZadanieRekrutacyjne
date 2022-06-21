#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "../headers/buffer.h"
#include "../headers/watchdog.h"

//Buffer przechowuje paczki wysyłane od readera do analyzera
// Zakładamy maksymalny rozmiar każdej z liczb z /proc/stat na 8 bajtów, czyli może mieć maksymalnie
// 20 znaków, dodając do tego string zawierający nazwe cpu, spacje itd zaokrąglamy do 256 charów
#define PACK_SIZE 256

//Ile pełnych transportów może pomieścić buffer
#define BUFFER_CAPACITY 6
//Wrapper na paczke od analyzera do printera
typedef struct Pack
{
    char pack[PACK_SIZE];
} Pack;

//Magazyn dla wątków Readera i Analyzera, by mogły przechowywać paczki, bez konieczności przetwarzania na bieżąco tego co
//Dostały
//Note: Operujemy na nim jak na ringed bufferze, ponieważ bezpośrednie odwoływanie się do elementu w tablicy nieznanego rozmiaru
//Nie jest bezpieczne
typedef struct Local_storage{
    size_t size;
    size_t head;
    size_t tail;
    Pack storage[]; //FAM
} Local_storage;
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

//Buffer współdzielony pomiędzy analyzerem i printerem,FAM jest po to, że urządzenie może mieć różną ilość rdzeni,
//A chcielibyśmy jednorazowo przesyłać informacje o zużyciu wszystkich procesorów na raz
typedef struct Results_buffer{
    size_t size;
    size_t head;
    size_t tail;
    size_t max_size;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    double buffer[]; //FAM
} Results_buffer;


Buffer* Buffer_create(const size_t buffer_size)
{
    Buffer* buffer = malloc(sizeof(*buffer) + sizeof(Pack)*buffer_size*BUFFER_CAPACITY);
    if(buffer == NULL)
        exit_on_error("MALLOC FAILED!\n");
    buffer->size = 0;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->max_size = buffer_size*BUFFER_CAPACITY;
    pthread_mutex_init(&(buffer->mutex),NULL);
    pthread_cond_init(&(buffer->can_consume), NULL);
    pthread_cond_init(&(buffer->can_produce), NULL);
    return buffer;
}

void Buffer_destroy(Buffer* const bf)
{
    pthread_cond_destroy(&bf->can_consume);
    pthread_cond_destroy(&bf->can_produce);
    pthread_mutex_destroy(&bf->mutex);

    free(bf);
}

bool Buffer_is_full(const Buffer* const bf)
{
    return bf->size == bf->max_size;
}

bool Buffer_is_empty(const Buffer* const bf)
{
    return bf->size == 0;
}

void Buffer_put(Buffer* const bf, const Pack* const pack)
{
    bf->buffer[bf->head] = *pack;
    bf->head = (bf->head + 1) % bf->max_size;
    ++bf->size;
}

Pack* Buffer_get(Buffer* const bf)
{
    Pack* pack = malloc(sizeof(Pack));
    if(pack == NULL)
        exit_on_error("MALLOC FAILED!\n");
    *pack = bf->buffer[bf->tail];
    bf->tail = (bf->tail + 1) % bf->max_size;
    --bf->size;

    return pack;
}

void Buffer_lock(Buffer* const bf)
{
    pthread_mutex_lock(&bf->mutex);
}

void Buffer_unlock(Buffer* const bf)
{
    pthread_mutex_unlock(&bf->mutex);
}

void Buffer_call_producer(Buffer* const bf)
{
    pthread_cond_signal(&bf->can_produce);
}

void Buffer_call_consumer(Buffer* const bf)
{
    pthread_cond_signal(&bf->can_consume);
}

void Buffer_wait_for_producer(Buffer* const bf)
{
    pthread_cond_wait(&bf->can_consume, &bf->mutex);
}

void Buffer_wait_for_consumer(Buffer* const bf)
{
    pthread_cond_wait(&bf->can_produce, &bf->mutex);
}


Pack* Pack_create(const char content[]){
    Pack* pc = malloc(sizeof(*pc));
    if (pc == NULL)
        exit_on_error("MALLOC FAILED!\n");
    *pc = (Pack){.pack = {0}};
    memcpy(&(pc->pack[0]),content,strlen(content)+1);
    return pc;
}

void Pack_destroy(Pack* const pc)
{
    free(pc);
}

char* Pack_get_content(Pack* const pc){
    return pc->pack;
}



Results_buffer* Results_buffer_create(const size_t buffer_size)
{

    Results_buffer* Results_buffer = malloc(sizeof(*Results_buffer) + sizeof(double)*buffer_size*BUFFER_CAPACITY);
    if (Results_buffer == NULL)
        return NULL;
    Results_buffer->size = 0;
    Results_buffer->head = 0;
    Results_buffer->tail = 0;
    Results_buffer->max_size = buffer_size*BUFFER_CAPACITY;
    pthread_mutex_init(&(Results_buffer->mutex),NULL);
    pthread_cond_init(&(Results_buffer->can_consume), NULL);
    pthread_cond_init(&(Results_buffer->can_produce), NULL);
    return Results_buffer;
}

void Results_buffer_destroy(Results_buffer* const bf)
{
    pthread_cond_destroy(&bf->can_consume);
    pthread_cond_destroy(&bf->can_produce);
    pthread_mutex_destroy(&bf->mutex);

    free(bf);
}

bool Results_buffer_is_full(const Results_buffer* const bf)
{
    return bf->size == bf->max_size;
}

bool Results_buffer_is_empty(const Results_buffer* const bf)
{
    return bf->size == 0;
}

void Results_buffer_put(Results_buffer* const bf, const double value)
{
    bf->buffer[bf->head] = value;
    bf->head = (bf->head + 1) % bf->max_size;
    ++bf->size;
}

double Results_buffer_get(Results_buffer* const bf)
{
    double value = bf->buffer[bf->tail];
    bf->tail = (bf->tail + 1) % bf->max_size;
    --bf->size;

    return value;
}

void Results_buffer_lock(Results_buffer* const bf)
{
    pthread_mutex_lock(&bf->mutex);
}

void Results_buffer_unlock(Results_buffer* const bf)
{
    pthread_mutex_unlock(&bf->mutex);
}

void Results_buffer_call_producer(Results_buffer* const bf)
{
    pthread_cond_signal(&bf->can_produce);
}

void Results_buffer_call_consumer(Results_buffer* const bf)
{
    pthread_cond_signal(&bf->can_consume);
}

void Results_buffer_wait_for_producer(Results_buffer* const bf)
{
    pthread_cond_wait(&bf->can_consume, &bf->mutex);
}

void Results_buffer_wait_for_consumer(Results_buffer* const bf)
{
    pthread_cond_wait(&bf->can_produce, &bf->mutex);
}



Local_storage* Local_storage_create(const size_t size){
    Local_storage* ls = malloc(sizeof(*ls) + sizeof(Pack)*size);
    if(ls == NULL)
        exit_on_error("MALLOC FAILED!\n");
    ls->size=size;
    ls->head=0;
    ls->tail=0;
    return ls;
}

void Local_storage_put(Local_storage* const ls,const Pack* const pack){
    ls->storage[ls->head] = *pack;
    ls->head = (ls->head + 1) % ls->size;
}

Pack* Local_storage_get(Local_storage* const ls){
    Pack* pack = malloc(sizeof(Pack));
    if(pack == NULL)
        exit_on_error("MALLOC FAILED!\n");
    *pack = ls->storage[ls->tail];
    ls->tail = (ls->tail + 1) % ls->size;

    return pack;
}

void Local_storage_destroy(Local_storage* const ls){
    free(ls);
}


int Log_message(Buffer* const logger, const char msg[static 1]){
    if(strlen(msg) > PACK_SIZE){
        return -1;
    }
    Pack* log = NULL;
    log = Pack_create(msg);
    Buffer_lock(logger);
    if(Buffer_is_full(logger)){
        Buffer_wait_for_consumer(logger);
    }
    Buffer_put(logger,log);
    Buffer_call_consumer(logger);
    Buffer_unlock(logger);
    Pack_destroy(log);
    return 0;
}

