#define BUFFER_SIZE 4096

typedef struct {
    char method[8];
    char path[129];
    char version[16];
    char headers[BUFFER_SIZE];
    int  content_length;
    int  expect;
} HTTPRequest;

/* Function that populates an HTTPRequest object with all of the metadata in the request.
 * 
 * Params: HTTPRequest* request             - request object to be filled
 *         char request_buffer[BUFFER_SIZE] - buffer containing request in HTTP format
 * 
 * Returns: int - status codes of 200(Ok), 400(Bad Request), or 413(Request too large)
 */
int convert_request(HTTPRequest*, char[BUFFER_SIZE]);

/* Function that verifies the URI of a request.
 * 
 * Params: char* path - URI path to be verified
 * 
 * Returns: int - status codes of 200(Ok) or 400(Bad Request)
 */
int uri_verification(char*);