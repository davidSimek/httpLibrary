#ifndef HTTP_LIBRARY_H
#define HTTP_LIBRARY_H

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


typedef struct {
    char method[10];
    char path[200];
    char version[10];
    Header* headerMap;
    size_t headerMapSize;
    char* body;
} HttpRequest;

typedef struct {
    char version[10];
    char status[10];
    char reasonPhrase[10];
    Header* headerMap;
    size_t headerMapSize;
    char* body;
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

int createServer(HttpConfig* config, int port);
void createRequest(HttpRequest* request, size_t maxHeaderCount, size_t maxSizeOfBody);
void createResponse(HttpResponse* response, size_t maxHeaderCount, size_t maxSizeOfBody);

void deleteServer(HttpConfig* config);
void deleteRequest(HttpRequest* request);
void deleteResponse(HttpResponse* response);

int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);
void sendResponse(HttpConfig* config, char* response, size_t responseLength);

void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);

void setVersion(HttpResponse* response, char* version);
void setStatus(HttpResponse* response, char* status);
void setReasonPhrase(HttpResponse* response, char* reasonPhrase);
void setHeader(Header* response, int index, char* key, char* value);

void fillResponse(HttpResponse* response, char* version, char* status, char* reasonPhrase, Header* headerMap, size_t headerMapSize, char* body);

size_t serializeResponse(HttpResponse* response, size_t responseLength, char* responseBuffer);

#endif
