#ifndef HTTP_LIBRARY_H
#define HTTP_LIBRARY_H

#include <stdlib.h>
#include <arpa/inet.h>

typedef struct {
    char method[10];
    char path[200];
    char version[10];
} HttpRequest;

typedef struct {
    char version[10];
    char status[10];
    char reasonPhrase[10];
} HttpResponse;

typedef struct {
    int port;
    int serverSocket;
    int clientSocket;
    int maxConntections;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;
} HttpConfig;

// setup
int setupServer(HttpConfig* config);
void cleanupServer(HttpConfig* config);

// in
int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);
void parseRequest(const HttpRequest* request, char* output, size_t bufferSize);

// out
void serializeResponse(char* input, const HttpResponse* response, size_t bufferSize);
void sendResponse(HttpConfig* config, char* response, size_t responseLength);

#endif
