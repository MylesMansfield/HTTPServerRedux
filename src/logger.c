#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "logger.h"
#include "interrupthandler.h"

#define MAX_CAPACITY 16

typedef struct {
    char content[256]; // Constructed text to be added to logfile
} Log;

typedef struct {
    int capacity;                // Max capacity of queue
    int size;                    // Number of node elements
    Log* elements[MAX_CAPACITY]; // Log* array of size capacity holding logs
    int head;                    // Index of head log
    int back;                    // Index of back log
    pthread_mutex_t mutex;       // Mutex lock used for buffer modification
    pthread_cond_t more;         // Thread signal for when more logs have been added
    pthread_cond_t less;         // Thread signal for when a log has been removed
} Logger;

void queue_log(char*);
void write_log();

Logger* logger;
FILE*  logfile;
char logfile_f[128] = "stderr";

void new_log(char* response) {
    char content[256] = "";

    time_t clk = time(NULL);
    char* time = ctime(&clk);

    strcat(content, time);
    content[strlen(content) - 1] = '\0';
    strcat(content, response);

    queue_log(content); // This will cause a hang if the queue fills up
}

void* new_logger(void* args) {
    char* filename = (char*) args;
    logger = (Logger*) malloc(sizeof(Logger));
    logfile = stderr;

    if ((int) strlen(filename) > 0) {
        logfile = fopen(filename, "w");
        strcpy(logfile_f, filename);
    }

    logger->capacity = MAX_CAPACITY;
    logger->head = 0;
    logger->back = 0;
    logger->size = 0;

    for (int i = 0; i < logger->capacity; i++) {
        logger->elements[i] = (Log*) malloc(sizeof(Log));
    }

    write_log();
    return NULL;
}

void free_logger() {
    for (int i = 0; i < logger->capacity; i++) {
        free(logger->elements[i]);
    }

    free(logger);
    if (strcmp(logfile_f, "stderr") != 0) { fclose(logfile); }
}

/* Function that queues content into logger in a thread safe manner.
 * 
 * Params: Log* log - log to be added to elements array
 * Returns: void
 */
void queue_log(char* log) {
    pthread_mutex_lock(&logger->mutex);
   
    while (logger->size >= logger->capacity) {
        pthread_cond_wait(&logger->less, &logger->mutex);
    }

    strcpy(logger->elements[logger->back++]->content, log); // Insert contents of log into back index of elements
    logger->back %= logger->capacity;
    logger->size++;

    pthread_cond_signal(&logger->more);
    pthread_mutex_unlock(&logger->mutex);
}

/* Function that commits logger elements to logfile in a thread safe manner.
 * 
 * Params: none
 * Returns: void
 */
void write_log() {
    while (interrupted == 0) {
        char log[256] = "UHOH";

        pthread_mutex_lock(&logger->mutex);
        while(logger->size <= 0) {
            pthread_cond_wait(&logger->more, &logger->mutex);
        }
        
        strcpy(log, logger->elements[logger->head++]->content);
        logger->head %= logger->capacity;
        logger->size--;

        pthread_cond_signal(&logger->less);
        pthread_mutex_unlock(&logger->mutex);

        // Write contents into logfile
        fprintf(logfile, "%s", log);
        fflush(logfile);
    }
}