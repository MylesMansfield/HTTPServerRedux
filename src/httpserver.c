#include "bind.h"
#include "httphandle.h"
#include "logger.h"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <sys/socket.h>

/* httpserver.c */
int main(int argc, char** argv) {
    if (argc == 1) { warnx("Usage: ./httpserver [-t threads] [-l logfile] <port>"); return 1; }

    char file_name[128] = "";
    int workerCount = 4;
    int opt;

    while ((opt = getopt(argc, argv, "l:t:")) != -1) {
        switch (opt) {
            case 't': workerCount = atoi(optarg); break;
            case 'l': strcpy(file_name, optarg); break;
            default: warnx("Usage: ./httpserver [-t threads] [-l logfile] <port>"); return 1;
        }
    }
    
    
    (void) workerCount; // TEMP TO REMOVE WARNING
    

    const uint16_t port = atoi(argv[argc - 1]);
    int socket_d = create_listen_socket(port); // Open socket

    if (socket_d < 0) { warnx("Socket Failed to Bind: %d\n", socket_d); return 1; }

    new_logger(file_name);

    // Server loop    
    while (1) {
        int file_d = accept(socket_d, NULL, NULL); // Accept connection
        handle_request(file_d);                    // Handle request
    }
    
    free_logger();
    close(socket_d);
    return 0;
}