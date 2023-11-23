# HTTP Library for C
## Overview

This C library provides a simple and easy-to-use framework for creating HTTP servers, parsing requests, and generating responses. The primary goal is to offer a lightweight solution for experimenting with HTTP server while providing enough control for you to make it YOUR way. The library is designed to be partly platform-independent, supporting both Linux and Windows environments. It has no dependencies on external libraries beyond what is provided by the system by default.

## Features

- **Easy Setup**: Quickly set up an HTTP server using a straightforward API.
- **Control**: Maintain control over your HTTP server while keeping the usage simple.
- **Cross-Platform**: Run your server seamlessly on both Linux and Windows.
- **Experimentation**: Ideal for students and developers looking to experiment with HTTP server.
- **Minimal Dependencies**: No external libraries are required beyond the system defaults.
- **Parsing and Serialization**: The library includes functions for parsing raw HTTP requests and serializing HTTP responses.

## API Reference
This is cut directly from httpLibrary.h.

```c
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
typedef struct {
    int port;
    SOCKET serverSocket;  // Change from int to SOCKET
    SOCKET clientSocket;  // Change from int to SOCKET
    int maxConnections;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int clientAddressLength;  // Change from socklen_t to int
} HttpConfig;


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

// Sets Header key and value
void setHeader(Header* response, int index, char* key, char* value);

// Fills HttpResponse
void fillResponse(HttpResponse* response, char* version, char* status, char* reasonPhrase, Header* headerMap, size_t headerMapSize, char* body);

// Serializes Http Response into response buffer so it can be sent
size_t serializeResponse(HttpResponse* response, size_t responseLength, char* responseBuffer);
```

## Example

```c
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
    Header* headerMap = (Header*)malloc(sizeof(Header) * 2);

    while (true) {
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

    free(headerMap);
    free(requestBuffer);
    free(responseBuffer);
    return 0;
}
```  
### logs it gave me:  
```
Server listening on port 8080
Connection accepted from 127.0.0.1:35242
Successfully recieved request.
RAW RESPONSE: HTTP/1.1 200 OK
thisServerIsMadyBy: David Simek


<h1>this is HOME</h1>
----------------------
Connection accepted from 127.0.0.1:34092
Successfully recieved request.
RAW RESPONSE: HTTP/1.1 200 OK
thisServerIsMadyBy: David Simek


<h1>you are in lol section now now</h1>
----------------------
```

## License
This project is licensed under GPL. Read more in LICENSE file.
