#include <stdio.h>
#include <string.h>
#include <err.h>

#include "httprequest.h"

int convert_request(HTTPRequest* request, char request_buffer[BUFFER_SIZE]) {
    char *saveptr, *header_line;
    int content_length = 0;

    char* ending = strstr(request_buffer, "\r\n\r\n");
    if (ending == NULL) { warnx("Request too large"); return 413; } // Request headers ending not found in buffer which means it's too long
    
    // Fill request line fields
    char* line = strtok_r(request_buffer, "\r\n", &saveptr);
    if (line != NULL && sscanf(line, "%s %s %s", request->method, request->path, request->version) == 3) { // Request line is found and parsed successfully
        request->content_length = -1;
        request->expect = 0;
        while ((header_line = strtok_r(NULL, "\r\n", &saveptr)) != NULL) { // Iterate through header lines adding to request.headers
            // TODO: Verify headers with if checking if %s: %s pattern is followed so long as header_line isn't empty
            if (sscanf(header_line, "Content-Length: %d", &content_length) == 1) { // Modify request.content_length appropriately
                request->content_length = content_length;
            } else if (strstr(header_line, "Expect: 100-continue") != NULL) {
                request->expect = 1;
            } else {
                // TODO: add header linked list additions here
                strcat(request->headers, header_line);
                strcat(request->headers, "\n");
            }
        }

        if (strcmp(request->method, "PUT") == 0 && request->content_length == -1) { warnx("No Content Length"); return 400; } // No "Content-Length: %d" found for PUT
        if (strcmp(request->method, "PUT") == 0 && request->expect == 0) { warnx("No Expect 100-continue"); return 400; }     // No "Expect: 100-continue" found

        return 200;
    } else  { warnx("Bad Request Line"); return 400; }
}

int uri_verification(char* path) {
    if (path == NULL) { warnx("Path doesn't exist"); return 400; }                  // Return 400 if path is non-existent
    if (!((int) strlen(path) > 1)) { warnx("Path isn't long enough"); return 400; } // Return 400 if path doesn't have more than 2 characters
    if (path[0] != '/') { warnx("Path doesn't start with '/'"); return 400; }       // Return 400 if path doesn't start with '/'

    for (int i = 0; i < (int) strlen(path); i++) { // Verify all chars in path are valid
        char letter = path[i];
        if (!(letter >= '-' && letter <= '9' ) && !(letter >= 'A' && letter <= 'Z') && !(letter >= 'a' && letter <= 'z')) {
            warnx("URI contains non approved characters");
            return 400;
        }
    }

    int period_count = 0;
    for (int i = 0; i < (int) strlen(path); i++) {
        if (path[i] == '.') { period_count++; }
    }

    if (period_count < 1) { warnx("URI doesn't have enough periods"); return 400; }           // Return 400 if path doesn't contain any periods
    if (strstr(path, "..") != NULL) { warnx("URI contains \"..\""); return 400; }             // Return 400 if path contains ".."
    if (strcmp(path, "/.gitignore") == 0) { warnx("URI cannot be gitignore"); return 400; }   // Return 400 if path is .gitignore
    if ((int) strlen(path) > 128) { warnx("URI is >128 characters"); return 400; }            // Return 400 if path is >128 chars

    return 200;
}