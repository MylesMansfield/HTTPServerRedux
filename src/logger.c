#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "logger.h"

#define MAX_CAPACITY 16

typedef struct {
    char content[256]; // Constructed text to be added to logfile
} Log;

typedef struct {
    int capacity;                // Max capacity of 
    int size;                    // Number of node elements
    Log* elements[MAX_CAPACITY]; // Log* array of size capacity holding logs
    int head;                    // Index of head Log
    int back;                    // Index of back log
} Logger;

/* Function that queues content into logger in a thread safe manner.
 * 
 * Params: Log* log - log to be added to elements array
 * Returns: void
 */
void queue_log(Log*); // This is where worker threads queue text to be logged

/* Function that commits logger elements to logfile in a thread safe manner.
 * 
 * Params: none
 * Returns: void
 */
void write_log(); // This is where logger thread will endlessly loop trying to write to logfile

Logger* logger;
FILE*  logfile;

void new_log(char* response) {
    Log* log = malloc(sizeof(Log));
    char content[256] = "";

    time_t clk = time(NULL);
    char* time = ctime(&clk);

    strcat(content, time);
    strcat(content, response);
    strcpy(log->content, content);

    queue_log(log);
}

void new_logger(char* filename) {
    logger = malloc(sizeof(Logger));
    logfile = stderr;

    if ((int) strlen(filename) > 0) {
        logfile = fopen(filename, "w");
    }
}

void free_logger() {
    // TODO: go through elements and free each log

    free(logger);
    fclose(logfile);
}

void queue_log(Log* log) {
    (void) log;
}

void write_log() {

}

/*

*/