#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>

#define REQUEST_MAX_LENGTH 1000

int main(int argc, char *argv[])
{
    HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
    HttpConfig config;
    setPort(&config, 8080);
    setupServer(&config);
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);
    //printf("request is :\n%s\n", requestBuffer);
    parseRequest(requestBuffer, requestLength, request);
    printf("Method is: %s\n", request->method);
    printf("Path is: %s\n", request->path);
    printf("Version is: %s\n", request->version);

    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    sendResponse(&config, response, strlen(response));
    cleanupServer(&config);

    free(request);
    free(requestBuffer);
    return 0;
}
