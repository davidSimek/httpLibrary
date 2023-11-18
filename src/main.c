#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#define REQUEST_MAX_LENGTH 1000
#define RESPONSE_MAX_LENGTH 2000

volatile bool shouldRun = true;

void handleCtrlC(int signum) {
    printf("Stopping server.");
    shouldRun = false;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handleCtrlC);

    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);
    char* responseBuffer = (char*)malloc(RESPONSE_MAX_LENGTH + 1);

    HttpRequest request;
    createRequest(&request, 100, 1000);
    HttpResponse response;
    createResponse(&response, 100, 1000);
    HttpConfig config;
    createServer(&config, 8080);
    Header* headerMap = (Header*)malloc(sizeof(Header) * 2);

    while (shouldRun) {
        size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);
        setHeader(headerMap, 0, "thisServerIsMadyBy", "David Simek");

        parseRequest(requestBuffer, requestLength, &request);
        
        if (strcmp(request.path, "/lol") == 0) {
            fillResponse(&response, "HTTP/1.1", "200", "OK", headerMap, 1, "<h1>you are in lol section now now</h1>");
        } else {
            fillResponse(&response, "HTTP/1.1", "200", "OK", headerMap, 1, "<h1>this is HOME</h1>");
        }
        size_t responseSize = serializeResponse(&response, RESPONSE_MAX_LENGTH, responseBuffer);
        printf("RAW RESPONSE: %s\n----------------------\n", responseBuffer);
        sendResponse(&config, responseBuffer, responseSize);
    }


    deleteServer(&config);
    deleteRequest(&request);
    deleteResponse(&response);

    free(requestBuffer);
    free(responseBuffer);
    return 0;
}
