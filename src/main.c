#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define REQUEST_MAX_LENGTH 1000

int main(int argc, char *argv[])
{
    // raw request is read into this
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    // create structure to parse raw data into
    HttpRequest request;
    createRequest(&request, 100, 1000);

    // create structure where all server data are stored and create server
    HttpConfig config;
    createServer(&config, 8080);

    // variable for storing count of bytes read
    size_t requestLength;

    // read request into buffer
    requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    // parse raw request into request structure
    parseRequest(requestBuffer, requestLength, &request);
    
    // generate response
    // you will be able to use HttpResponse structure with serializeResponse() function to create response easily, not implemented for now.
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";

    // send raw response to client
    sendResponse(&config, response, strlen(response));

    // clean-up
    deleteServer(&config);
    deleteRequest(&request);
    free(requestBuffer);
    return 0;
}
