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
void* new_logger(void*);
void free_logger();