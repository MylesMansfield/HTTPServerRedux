#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "threadpool.h"
#include "httphandle.h"
#include "interrupthandler.h"

#define MAX_CAPACITY 7

typedef struct {
    int capacity;                    // Max capacity of pool
    int size;                        // Number of file descriptors in queue
    pthread_t workers[MAX_CAPACITY]; // pthread array of size capacity holding pthreads that consume file descriptors
    int elements[MAX_CAPACITY];      // int array acting as a queue of file descriptors for the workers to consume
    int head;                        // Index of head descriptor
    int back;                        // Index of back descriptor
    pthread_mutex_t mutex;           // Mutex lock used for buffer modification
    pthread_cond_t more;             // Thread signal for when more descriptors have been added
    pthread_cond_t less;             // Thread signal for when a descriptor has been removed
} Threadpool;

void* dequeue_pool();

Threadpool* pool;

void create_pool(int worker_count) {
    pool = (Threadpool*) malloc(sizeof(Threadpool));

    pool->capacity = worker_count;
    pool->size = 0;
    pool->head = 0;
    pool->back = 0;

    for (int i = 0; i < worker_count; i++) {
        pthread_create(&(pool->workers[i]), NULL, dequeue_pool, NULL);
    }
}

void free_pool() {
    free(pool);
}

void queue_pool(int file_d) {
    pthread_mutex_lock(&pool->mutex);
   
    while (pool->size >= pool->capacity) {
        pthread_cond_wait(&pool->less, &pool->mutex);
    }

    pool->elements[pool->back++] = file_d;
    pool->back %= pool->capacity;
    pool->size++;

    pthread_cond_signal(&pool->more);
    pthread_mutex_unlock(&pool->mutex);
}

/* Function that every worker thread runs which loops till interrupt trying to dequeue the next element from elements.
 * 
 * Params: none
 * Returns: void
 */
void* dequeue_pool() {
    printf("Started Worker\n\n");
    fflush(stdout);

    while (pool->size > 0 || interrupted == 0) {
        pthread_mutex_lock(&pool->mutex);

        while(pool->size <= 0) {
            pthread_cond_wait(&pool->more, &pool->mutex);
        }
        
        int file_d = pool->elements[pool->head++];
        pool->head %= pool->capacity;
        pool->size--;

        pthread_cond_signal(&pool->less);
        pthread_mutex_unlock(&pool->mutex);

        // Handle request on file_d
        handle_request(file_d);
    }
    return NULL;
}