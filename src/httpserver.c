#include "bind.h"
#include "threadpool.h"
#include "logger.h"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <pthread.h>

/* httpserver.c */
int main(int argc, char** argv) {
    if (argc == 1) { warnx("Usage: ./httpserver [-t threads] [-l logfile] <port>"); return 1; }

    char file_name[128] = "logfile.txt";
    int worker_count = 4;
    int opt;

    while ((opt = getopt(argc, argv, "l:t:")) != -1) {
        switch (opt) {
            case 't': worker_count = atoi(optarg); break;
            case 'l': strcpy(file_name, optarg); break;
            default: warnx("Usage: ./httpserver [-t threads] [-l logfile] <port>"); return 1;
        }
    }

    if (worker_count > 7) { warnx("Max worker thread capacity of 7 exceeded"); return 1; }

    const uint16_t port = atoi(argv[argc - 1]);
    int socket_d = create_listen_socket(port); // Open socket

    if (socket_d < 0) { warnx("Socket Failed to Bind: %d\n", socket_d); return 1; }

    pthread_t logger_thread;
    pthread_create(&logger_thread, NULL, new_logger, (void*)file_name); // Creates logger thread

    create_pool(worker_count); // Creates threadpool with worker_count number of threads
    
    // Server loop    
    while (1) {
        int file_d = accept(socket_d, NULL, NULL); // Accept connection
        queue_pool(file_d);                        // Handle request
    }

    pthread_join(logger_thread, NULL); // TODO: add signal handling for SIGINT where upon interrupt our logger contents are logged then gracefully freed
    free_logger();
    free_pool();

    close(socket_d);
    return 0;
}