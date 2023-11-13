#ifndef HTTP_LIBRARY_H
#define HTTP_LIBRARY_H

#define REQUEST_MAX_LENGTH 1000
#define HEADER_MAP_LENGTH 50
#define BODY_LENGTH 1000


#include <stdlib.h>

#ifdef WINDOWS 
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

typedef struct {
    char key[30];
    char value[200];
} Header;

typedef Header HeaderMap[HEADER_MAP_LENGTH];

typedef struct {
    char method[10];
    char path[200];
    char version[10];
    HeaderMap headerMap;
    char* body;
} HttpRequest;

typedef struct {
    char version[10];
    char status[10];
    char reasonPhrase[10];
} HttpResponse;

#ifdef WINDOWS
typedef struct {
    int port;
    SOCKET serverSocket;  // Change from int to SOCKET
    SOCKET clientSocket;  // Change from int to SOCKET
    int maxConnections;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int clientAddressLength;  // Change from socklen_t to int
} HttpConfig;
#else
typedef struct {
    int port;
    int serverSocket;
    int clientSocket;
    int maxConntections;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;
} HttpConfig;   
#endif

// setup
int setupServer(HttpConfig* config);
void cleanupServer(HttpConfig* config);
void setPort(HttpConfig* config, int port);

int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);
void sendResponse(HttpConfig* config, char* response, size_t responseLength);

void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);
void serializeResponse();

#endif
