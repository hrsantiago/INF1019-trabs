#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "messenger.h"

struct synch {
    int buffer[1];
    int messages;
};

struct asynch {
    int *buffer;
    int messages;
    int capacity;
    sem_t recvSem;
    sem_t bufferMutex;
};

synch_t *create_new_s()
{
    synch_t *handler = (synch_t*)malloc(sizeof(synch_t));
    handler->messages = 0;
    return handler;
}

asynch_t *create_new_a(int capacity)
{
    asynch_t *handler = (asynch_t*)malloc(sizeof(asynch_t));
    handler->buffer = (int*)malloc(capacity * sizeof(int));
    handler->capacity = capacity;
    handler->messages = 0;
    sem_init(&handler->recvSem, 0, 0);
    sem_init(&handler->bufferMutex, 0, 1);
    return handler;
}

void destroy(synch_t *h)
{
    //sem_destroy(&h->mutex);
    //free(h->buffer);
    free(h);
}

void destroy_a(asynch_t *h)
{
    sem_destroy(&h->recvSem);
    sem_destroy(&h->bufferMutex);
    free(h->buffer);
    free(h);
}

int send(synch_t *h, int *message)
{

}

int recv(synch_t *h, int *message)
{

}

int asend(asynch_t *h, int *message)
{
    sem_wait(&h->bufferMutex);
    if(h->messages == h->capacity) {
        sem_post(&h->bufferMutex);
        return 0;
    }

    h->buffer[h->messages] = *message;
    ++h->messages;
    sem_post(&h->bufferMutex);
    sem_post(&h->recvSem);
    return 1;
}

int arecv(asynch_t *h, int *message)
{
    sem_wait(&h->recvSem);

    sem_wait(&h->bufferMutex);
    *message = h->buffer[0];
    --h->messages;
    memmove(h->buffer, h->buffer+1, h->messages * sizeof(int)); // not very efficient but it does not matter
    sem_post(&h->bufferMutex);
    return 1;
}
