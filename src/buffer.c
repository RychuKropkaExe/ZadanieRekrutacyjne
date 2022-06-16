#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <buffer.h>

//Buffer przechowuje paczki wysyłane od readera do analyzera
#define BUFFER_SIZE 6
//Paczka przechowuje plain texty wszystkich rdzeni z /proc/stat
#define PACK_SIZE 9
// Zakładamy maksymalny rozmiar każdej z liczb z /proc/stat na 8 bajtów, czyli może mieć maksymalnie
// 20 znaków, dodając do tego string zawierający nazwe cpu, spacje itd zaokrąglamy do 250 charów
#define TEXT_SIZE 250

//Wrapper na plain text
typedef struct Plain_text
{
    char plain_text[TEXT_SIZE];
}Plain_text;

//Wrapper na paczke
typedef struct Pack
{
    Plain_text pack[PACK_SIZE];
} Pack;

//Buffer współdzielony pomiędzy readerem i Analyzerem
typedef struct Buffer
{   
    Pack buffer[BUFFER_SIZE];
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;

} Buffer;