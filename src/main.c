#include <stdio.h>
#include "httpLibrary.h"
#include <string.h>

#define REQUEST_MAX_LENGTH 1000

int main(int argc, char *argv[])
{
    HttpConfig config;
    config.port = 8080;
    setupServer(&config);
    char* request = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    size_t requestLength = getRequest(&config, request, REQUEST_MAX_LENGTH);
    printf("request is :\n%s\n", request);

    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    sendResponse(&config, response, strlen(response));
    cleanupServer(&config);

    return 0;
}
