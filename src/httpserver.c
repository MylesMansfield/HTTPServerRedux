#include "bind.h"
#include "httphandle.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
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
    
    new_logger(file_name);


    (void) workerCount; // TEMP TO REMOVE WARNING
    

    const uint16_t port = atoi(argv[argc - 1]);
    int socket_d = create_listen_socket(port); // Open socket

    if (socket_d < 0) { warnx("Socket Failed to Bind: %d\n", socket_d); return 1; }

    char request_buffer[BUFFER_SIZE] = "";

    // Server loop    
    while (1) {
        int file_d = accept(socket_d, NULL, NULL); // Accept connection
        int code = 200;

        // Read request + headers
        strcpy(request_buffer, "");
        read(file_d, request_buffer, BUFFER_SIZE);

        // Convert network request to HTTPRequest object
        HTTPRequest* request = malloc(sizeof(HTTPRequest));
        code = convert_request(request, request_buffer);

        // Verify URI
        if (code == 200) { code = uri_verification(request->path); }

        // Make appropriate method call
        if (code == 200 && strcmp(request->version,"HTTP/1.1") == 0) { // Is version valid
            if (strcmp(request->method,"GET") == 0) {
                code = handle_get(request, file_d);
            } else if (strcmp(request->method,"HEAD") == 0) {
                code = handle_head(request, file_d);
            } else if (strcmp(request->method,"PUT") == 0) {
                code = handle_put(request, file_d);
            } else if (strcmp(request->method,"DELETE") == 0) {
                code = handle_delete(request, file_d);
            } else {
                code = 501;
            }
        }

        printf("%s %s %s\n", request->method, request->path, request->version);
        printf("STATUS CODE: %d\n\n", code);
        free(request);
        handle_response(file_d, code); // Handles response appropriately
    }
    
    free_logger();
    close(socket_d);
    return 0;
}