#ifndef MESSENGER_H
#define MESSENGER_H

#include <semaphore.h>

struct synch {
    int buffer[1];
    sem_t sendSem;
    sem_t recvSem;
};
typedef struct synch synch_t;

struct asynch {
    int *buffer;
    int messages;
    int capacity;
    sem_t recvSem;
    sem_t bufferMutex;
};
typedef struct asynch asynch_t;

/// Create a new synchronous channel and return a Handle
void create_new_s(synch_t *handler);

/// Create a new asynchronous channel with certain capacity and return a Handle
void create_new_a(asynch_t *handler, int capacity);

/// Free a synchronous channel
void destroy(synch_t *h);

/// Free an asynchronous channel
void destroy_a(asynch_t *h);

/// Send a message through a synchronous channel with capacity 1
int send(synch_t *h, int *message);

/// Receive a message through a synchronous channel
int recv(synch_t *h, int *message);

/// Send a message through an asynchronous channel
/// If buffer is full, returns with an error
int asend(asynch_t *h, int *message);

/// Receive a message through an asynchronous channel
/// If buffer is empty, blocks current thread
int arecv(asynch_t *h, int *message);

#endif
