#ifndef HTTP_LIBRARY_H
#define HTTP_LIBRARY_H

#include <stdlib.h>

#ifdef WINDOWS 
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

// Represents Key-Value pair of HTTP header
typedef struct {
    char key[30];
    char value[200];
} Header;


// Represents HTTP request
typedef struct {
    char method[10];
    char path[200];
    char version[10];
    Header* headerMap;
    size_t headerMapSize;
    char* body;
    size_t maxSizeOfBody;
} HttpRequest;

// Represents HTTP response
typedef struct {
    char version[10];
    char status[10];
    char reasonPhrase[10];
    Header* headerMap;
    size_t headerMapSize;
    char* body;
} HttpResponse;

// Represents HTTP server config for Windows
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


// Represents HTTP server config for Linux
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

// Initializes HTTP server variables and "starts it up"
int createServer(HttpConfig* config, int port);

// Initializes HTTP request structure
void createRequest(HttpRequest* request, size_t maxHeaderCount, size_t maxSizeOfBody);

// Initializes HTTP response structure
void createResponse(HttpResponse* response, size_t maxHeaderCount, size_t maxSizeOfBody);

// Cleans up HTTP server variables and "turns it off"
void deleteServer(HttpConfig* config);

// Cleand up after HttpRequest and HttpResponse structs
void deleteRequest(HttpRequest* request);
void deleteResponse(HttpResponse* response);

// Reads HTTP request into request buffer
int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);
// Writes HTTP response from response buffer
void sendResponse(HttpConfig* config, char* response, size_t responseLength);

// Parses raw request into request structure
void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);

// Manipulates HttpResponse
// You will probably not need this
void setVersion(HttpResponse* response, char* version);
void setStatus(HttpResponse* response, char* status);
void setReasonPhrase(HttpResponse* response, char* reasonPhrase);
void setHeader(Header* response, int index, char* key, char* value);

// Fills HttpResponse
void fillResponse(HttpResponse* response, char* version, char* status, char* reasonPhrase, Header* headerMap, size_t headerMapSize, char* body);

// Serializes Http Response into response buffer so it can be sent
size_t serializeResponse(HttpResponse* response, size_t responseLength, char* responseBuffer);

#endif
