#include "httphandle.h"
#include "logger.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>

const char* get_code_200 = "HTTP/1.1 200 OK\r\nContent-Length: ";

const char* code_100 = "HTTP/1.1 100 Continue\r\n\r\n";
const char* code_200 = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOk\n";
const char* code_201 = "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n";
const char* code_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
const char* code_403 = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
const char* code_404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
const char* code_413 = "HTTP/1.1 413 Request Entity Too Large\r\nContent-Length: 25\r\n\r\nRequest Entity Too Large\n";
const char* code_500 = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal Server Error\n";
const char* code_501 = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n";

int handle_head(HTTPRequest* request, int file_d) {
    char path[138] = "serverdir"; // 138 b/c 128 max length path + length of "serverdir" + path + '\0' at end
    strcat(path, request->path);

    int get_fd = open(path, O_RDONLY);
    if (get_fd == -1) {
        if (errno == EACCES) { return 403; }      // Failed to open b/c no access
        else if (errno == ENOENT) { return 404; } // Failed to find
        else { return 500; }                      // Failed b/c other
    }

    char response[BUFFER_SIZE] = "";
    int content_length = 0;
    int len_read = 0;

    while ((len_read = read(get_fd, response, BUFFER_SIZE)) > 0) {
        content_length += len_read;
    }
    
    close(get_fd);

    char num_buff[20] = "";

    strcpy(response, get_code_200);
    sprintf(num_buff, "%d", content_length);
    strcat(response, num_buff);
    strcat(response, "\r\n\r\n");

    write(file_d, response, strlen(response));
    return 200;
}

int handle_get(HTTPRequest* request, int file_d) {
    char path[138] = "serverdir"; // 138 b/c 128 max length path + length of "serverdir" + path + '\0' at end
    strcat(path, request->path);
    
    int code = handle_head(request, file_d);
    if (code != 200) { return code; } // Return code from handle_head() if failure

    int get_fd = open(path, O_RDONLY);
    if (get_fd == -1) {
        if (errno == EACCES) { return 403; }      // Failed to open b/c no access
        else if (errno == ENOENT) { return 404; } // Failed to find
        else { return 500; }                      // Failed b/c other
    }

    char response[BUFFER_SIZE] = "";
    int len_read = 0;

    while ((len_read = read(get_fd, response, BUFFER_SIZE)) > 0) {
        write(file_d, response, len_read);
    }
    
    close(get_fd);
    return 200;
}

int handle_put(HTTPRequest* request, int file_d) {
    char path[138] = "serverdir"; // 138 b/c 128 max length path + length of "serverdir" + path + '\0' at end
    strcat(path, request->path);
    int code = 200;

    int put_fd = open(path, O_WRONLY | O_TRUNC);
    if (put_fd == -1) {
        if (errno == EACCES) { return 403; } // Failed to open b/c no access
        else if (errno == ENOENT) {          // Creates file if it doesn't exist
            code = 201;
            put_fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        }
        else { return 500; }                 // Failed b/c other
    }

    write(file_d, code_100, strlen(code_100)); // Respond with 100 confirmation

    char buffer[BUFFER_SIZE] = "";
    int len_read = 0, len_copied = 0;

    while ((len_read = read(file_d, buffer, BUFFER_SIZE)) > 0) {
        write(put_fd, buffer, len_read);

        len_copied += len_read;
        if (len_copied >= request->content_length) { break; }
    }
    close(put_fd);

    if (code == 200) { write(file_d, code_200, strlen(code_200)); }
    else { write(file_d, code_201, strlen(code_201)); }
    
    return code;
}

int handle_delete(HTTPRequest* request, int file_d) {
    char path[138] = "serverdir"; // 138 b/c 128 max length path + length of "serverdir" + path + '\0' at end
    strcat(path, request->path);

    if (remove(path) == -1) {
        if (errno == EACCES) { return 403; }      // Failed to remove b/c no access
        else if (errno == ENOENT) { return 404; } // Failed to find
        else { return 500; }                      // Failed b/c other
    }

    write(file_d, code_200, strlen(code_200));
    return 200;
}

void handle_response(int file_d, int code) {
    if(code == 200 || code == 201) {} // Already handled
    else if (code == 400) { write(file_d, code_400, strlen(code_400)); }
    else if (code == 403) { write(file_d, code_403, strlen(code_403)); }
    else if (code == 404) { write(file_d, code_404, strlen(code_404)); }
    else if (code == 413) { write(file_d, code_413, strlen(code_413)); }
    else if (code == 501) { write(file_d, code_501, strlen(code_501)); }
    else { write(file_d, code_500, strlen(code_500)); } // Code 500 SERVER ERROR

    close(file_d);
}