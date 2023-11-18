# HTTP Library for C
### !!!
It does't compile library for now, it is easier for me to have it like this. I want to implement function for response serialization, and then transfer it into library.

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

#### int createServer(HttpConfig* config, int port);
sets up HttpConfig
#### void createRequest(HttpRequest* request, size_t maxHeaderCount, size_t maxSizeOfBody);
sets up HttpRequest
#### void createResponse(HttpResponse* response, size_t maxHeaderCount, size_t maxSizeOfBody);
sets up HttpResponse

#### void deleteServer(HttpConfig* config);
deletes HttpConfig
#### void deleteRequest(HttpRequest* request);
deletes HttpRequest
#### void deleteResponse(HttpResponse* response);
deletes HttpResponse

#### int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);
copies request into char* request and returns length
#### void sendResponse(HttpConfig* config, char* response, size_t responseLength);
sends response from char* response

#### void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);
parses raw request into HttpRequest structure

#### void setVersion(HttpResponse* response, char* version);
#### void setStatus(HttpResponse* response, char* status);
#### void setReasonPhrase(HttpResponse* response, char* reasonPhrase);
you will probably not need these  
these set single fields of HttpResponse

#### void setHeader(Header* response, int index, char* key, char* value);
sets header in headerMap  
you are responsible for array overflow here  
just be careful with index

#### void fillResponse(HttpResponse* response, char* version, char* status, char* reasonPhrase, Header* headerMap, size_t headerMapSize, char* body);
this fills response struct in really accessible way

#### size_t serializeResponse(HttpResponse* response, size_t responseLength, char* responseBuffer);
this serializes HttpResponse struct into responseBuffe so you can send it


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
