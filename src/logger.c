#include "logger.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

/*
typedef struct {
    char content[256];
} Log;

typedef struct {
    int capacity;                // Max capacity of 
    int size;                    // Number of node elements
    Log* elements[MAX_CAPACITY]; // Log* array of size capacity holding logs
    int head;                    // Index of head Log
    int back;                    // Index of back log
} Logger;
*/

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

}

void write_log() {

}

/*

*/