#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // allocate space for request struct
    HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
    // allocate space for body of request
    request->body = (char*)malloc(BODY_LENGTH);
    // allocate buffer to request into
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    // setup all variables and "run" server
    HttpConfig config;
    setPort(&config, 8080);
    setupServer(&config);

    // get request
    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    // parse request
    parseRequest(requestBuffer, requestLength, request);

    //---------------
    // DO YOUR STUFF
    //---------------
    
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";

    // send response
    sendResponse(&config, response, strlen(response));

    // "stop" server
    cleanupServer(&config);

    // for now you are responsible for all allocations
    // my plan is to mange this, trying to think of the best way to execute this
    free(request->body);
    free(request);
    free(requestBuffer);
    return 0;
}
