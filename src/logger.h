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

/* Function that creates new Log object with added timestamp and queue's it for logger.
 * 
 * Params: char* response - response string with <Oper> <URI> <Status-Code>\n
 * Returns: void
 */
void new_log(char*); // This is where worker threads turn response into log and attempt to queue

/* Functions that initialize and free logger object.
 * 
 * Params: char* filename - given name for logfile, default is stderr if invalid 
 * Returns: void
 */
void new_logger(char*);
void free_logger();

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