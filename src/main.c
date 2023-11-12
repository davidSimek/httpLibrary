#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>

#define REQUEST_MAX_LENGTH 1000

int main(int argc, char *argv[])
{
    HttpConfig config;
    setPort(&config, 8080);
    setupServer(&config);
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);
    printf("request is :\n%s\n", requestBuffer);

    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    sendResponse(&config, response, strlen(response));
    cleanupServer(&config);

    free(requestBuffer);
    return 0;
}
