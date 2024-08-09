#include "httprequest.h"

/* Function that responds to HEAD requests(used by GET aswell). Returns the headers of a GET call.
 * Function that responds to GET requests. Returns the contents of file at path.
 * Function that responds to PUT requests. Replaces content of file at path or creates new file if it doesn't already exist.
 * Function that responds to DELETE requests. Deletes the file at path if it exists.
 * 
 * Params: HTTPRequest* request - object containing the parsed request message
 *         int file_d           - file descriptor of user
 * 
 * Returns: int - status codes of 200(Ok), 403(Forbidden), 404(Not Found), or 500(Internal Server Error)
 *          int - status codes of 200(Ok), 403(Forbidden), 404(Not Found), or 500(Internal Server Error)
 *          int - status codes of 200(Ok), 201(Created), 403(Forbidden), or 500(Internal Server Error)
 *          int - status codes of 200(Ok), 403(Forbidden), 404(Not Found), or 500(Internal Server Error)
 */
int handle_head(HTTPRequest*, int);
int handle_get(HTTPRequest*, int);
int handle_put(HTTPRequest*, int);
int handle_delete(HTTPRequest*, int);

/* Function that responds to user with appropriate message.
 * 
 * Params: int file_d - file descriptor of user
 *         int code   - status code that the server is to respond with
 * 
 * Returns: void
 */
void handle_response(int, int);