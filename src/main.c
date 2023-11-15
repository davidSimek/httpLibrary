#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    HttpRequest request;
    createRequest(&request, 100, 1000);
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    HttpConfig config;
    setupServer(&config, 8080);

    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    parseRequest(requestBuffer, requestLength, &request);
    
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";

    sendResponse(&config, response, strlen(response));

    cleanupServer(&config);

    deleteRequest(&request);
    free(requestBuffer);
    return 0;
}
