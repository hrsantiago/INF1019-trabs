#include <pthread.h>
#include <stdlib.h>
#include "messenger.h"

struct synch {
    int buffer[1];
    int messages;
};

struct asynch {
    int *buffer;
    int messages;
    int capacity;
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
    return handler;
}

int send(synch_t *h, int *message)
{

}

int recv(synch_t *h, int *message)
{

}

int asend(asynch_t *h, int *message)
{

}

int arecv(asynch_t *h, int *message)
{

}
