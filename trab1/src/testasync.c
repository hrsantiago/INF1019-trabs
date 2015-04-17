#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "testasync.h"
#include "messenger.h"

#define msleep(t) usleep(t * 1000)

struct params {
    int threadId;
    asynch_t *messenger;
};
typedef struct params params_t;

// Ensure that producers * totalMessagesPerProducer / consumers is an integer, so the program will finish properly.
static const int totalMessagesPerProducer = 50;
static const int producers = 2;
static const int consumers = 5;
static const int capacity = 4;
static const int producerSleepMs = 250;
static const int consumerSleepMs = 100;

static void *producer(void *vparams)
{
    params_t *params = (params_t*)vparams;
    int i;
    int message;
    printf("Producer %d started\n", params->threadId);
    for(i = 0; i < totalMessagesPerProducer; ++i) {
        message = i+((params->threadId-1) * totalMessagesPerProducer);
        if(asend(params->messenger, &message) == 0) {
            printf("Producer %d failed to send message %d\n", params->threadId, message);
            --i;
        }
        else
            printf("Producer %d send message %d\n", params->threadId, message);
        msleep(producerSleepMs);
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
    printf("Consumer %d started\n", params->threadId);
    for(i = 0; i < (producers * totalMessagesPerProducer) / consumers; ++i) {
        arecv(params->messenger, &message);
        printf("Consumer %d recv message %d\n", params->threadId, message);
        msleep(consumerSleepMs);
    }
    printf("Consumer %d finished\n", params->threadId);
    free(params);
    return NULL;
}

void runAsyncTest()
{
    int i;
    printf("Running async test...\n");

    pthread_t *threads = (pthread_t*)malloc((producers+consumers)*sizeof(pthread_t));

    asynch_t *messenger = create_new_a(capacity);
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

    destroy_a(messenger);
    free(threads);
}
