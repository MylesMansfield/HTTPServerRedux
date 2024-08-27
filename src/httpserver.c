#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>

#include "bind.h"
#include "threadpool.h"
#include "logger.h"
#include "interrupthandler.h"

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

    create_pool(worker_count);     // Creates threadpool with worker_count number of threads

    signal(SIGINT, handle_sigint); // Setup SIGINT handler
    
    // Server loop    
    while (interrupted == 0) {
        int file_d;
        struct pollfd fds[1];      
        fds[0].fd = socket_d;
        fds[0].events = POLLIN;

        int poll_val = poll(fds, 1, 1);

        if (poll_val <= 0) { continue; }

        file_d = accept(socket_d, NULL, NULL); // Accept connection
        if (file_d != -1) {
            queue_pool(file_d);                // Handle request
        } 
    }

    free_logger();
    free_pool();
    close(socket_d);

    return 0;
}