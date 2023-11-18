#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define REQUEST_MAX_LENGTH 1000
#define RESPONSE_MAX_LENGTH 2000

void displayRequest(HttpRequest* request);

int main(int argc, char *argv[])
{
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);
    char* responseBuffer = (char*)malloc(RESPONSE_MAX_LENGTH + 1);

    HttpRequest request;
    createRequest(&request, 100, 1000);
    HttpResponse response;
    createResponse(&response, 100, 1000);
    HttpConfig config;
    createServer(&config, 8080);

    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    parseRequest(requestBuffer, requestLength, &request);

    displayRequest(&request);
    
    Header* headerMap = (Header*)malloc(sizeof(Header) * 1);
    setHeader(headerMap, 0, "name", "gaga");
    fillResponse(&response, "HTTP/1:1", "222", "LOL", headerMap, 1, "<h1>hello this is server</h1>");

    char* rawResponse = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    sendResponse(&config, rawResponse, strlen(rawResponse));


    deleteServer(&config);
    deleteRequest(&request);
    deleteResponse(&response);

    free(requestBuffer);
    free(responseBuffer);
    return 0;
}

void displayRequest(HttpRequest* request) {
    printf("METHOD:  %s\n", request->method);
    printf("PATH:    %s\n", request->path);
    printf("VERSION: %s\n", request->version);

    for (int i = 0; i < request->headerMapSize; i++) {
        if (request->headerMap[i].key[0] == 0)
            break;

        printf("Header(%d) %s : %s\n", i, request->headerMap[i].key, request->headerMap[i].value);
    }

    printf("body: %s", request->body);
}
