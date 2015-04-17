#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "testsync.h"
#include "messenger.h"

#define msleep(t) usleep(t * 1000)

struct params {
    int threadId;
    synch_t *messenger;
};
typedef struct params params_t;

// ensure that producers * totalMessages / consumers is an integer, so the program will finish properly.
static const int totalMessages = 50;
static const int producers = 3;
static const int consumers = 5;

static void *producer(void *vparams)
{
    params_t *params = (params_t*)vparams;
    int i;
    int message;
    printf("Producer %d started\n", params->threadId);
    for(i = 0; i < totalMessages; ++i) {
        message = i+((params->threadId-1) * totalMessages);
        send(params->messenger, &message);
        printf("Producer %d send message %d\n", params->threadId, message);
        msleep(250);
    }
    printf("Producer %d finished\n", params->threadId);
    free(params);
    return NULL;
}

static void *consumer(void *vparams)
{
    params_t *params = (params_t*)vparams;
    int i;
    int message;
    sleep(1);
    printf("Consumer %d started\n", params->threadId);
    for(i = 0; i < (producers * totalMessages) / consumers; ++i) {
        recv(params->messenger, &message);
        printf("Consumer %d recv message %d\n", params->threadId, message);
        msleep(100);
    }
    printf("Consumer %d finished\n", params->threadId);
    free(params);
    return NULL;
}

void runSyncTest()
{
    int i;
    printf("Running sync test...\n");

    pthread_t *threads = (pthread_t*)malloc((producers+consumers)*sizeof(pthread_t));

    synch_t *messenger = create_new_s();
    if(messenger == NULL) {
        printf("Error allocating messenger\n");
        return;
    }

    for(i = 0; i < producers; ++i) {
        params_t *producerParams = (params_t*)malloc(sizeof(params_t));
        producerParams->threadId = i+1;
        producerParams->messenger = messenger;
        if(pthread_create(&threads[i], NULL, producer, producerParams)) {
            printf("Error creating producer thread\n");
            return;
        }
    }

    for(i = 0; i < consumers; ++i) {
        params_t *consumerParams = (params_t*)malloc(sizeof(params_t));
        consumerParams->threadId = i+1;
        consumerParams->messenger = messenger;
        if(pthread_create(&threads[producers+i], NULL, consumer, consumerParams)) {
            printf("Error creating producer thread\n");
            return;
        }
    }

    for(i = 0; i < producers + consumers; ++i) {
        if(pthread_join(threads[i], NULL)) {
            printf("Error joining thread\n");
            return;
        }
    }

    destroy(messenger);
    free(threads);
}
