/* Functions that create and free worker threads in threadpool pattern.
 * 
 * Params: int worker_count - number of worker threads to create
 *         none
 * Returns: void
 */
void create_pool(int);
void free_pool();

/* Function that enqueues a newly connected file descriptor to elements array.
 * 
 * Params: int file_d - file descriptor of new connection
 * Returns: void
 */
void queue_pool(int);