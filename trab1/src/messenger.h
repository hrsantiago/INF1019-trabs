#ifndef MESSENGER_H
#define MESSENGER_H

typedef struct synch synch_t;
typedef struct asynch asynch_t;

/// Create a new synchronous channel and return a Handle
synch_t *create_new_s();

/// Create a new asynchronous channel with certain capacity and return a Handle
asynch_t *create_new_a(int capacity);

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
