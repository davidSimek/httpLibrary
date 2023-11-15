# HTTP Library for C

## Overview

This C library provides a simple and easy-to-use framework for creating HTTP servers, parsing requests, and generating responses. The primary goal is to offer a lightweight solution for experimenting with HTTP server while providing enough control for you to make it YOUR way. The library is designed to be partly platform-independent, supporting both Linux and Windows environments. It has no dependencies on external libraries beyond what is provided by the system by default.

## Features

- **Easy Setup**: Quickly set up an HTTP server using a straightforward API.
- **Control**: Maintain control over your HTTP server while keeping the usage simple.
- **Cross-Platform**: Run your server seamlessly on both Linux and Windows.
- **Experimentation**: Ideal for students and developers looking to experiment with HTTP server.
- **Minimal Dependencies**: No external libraries are required beyond the system defaults.
- **Parsing and Serialization**: The library includes functions for parsing raw HTTP requests and will include function for serializing HTTP responses.

## API Reference

### `int createServer(HttpConfig* config, int port);`

Initialize and set up the HTTP server running on provided port.

### `void deleteServer(HttpConfig* config);`

Clean up and release resources associated with the HTTP server.

### `int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);`

Receive an HTTP request from the client and store it in the provided buffer.


### `void createRequest(HttpRequest* request, size_t maxHeaderCount, size_t maxSizeOfBody);`

Initialize structure where request is parsed into.

### `void deleteRequest(HttpRequest* request);`

Clean up structure where for request.

### `void sendResponse(HttpConfig* config, char* response, size_t responseLength);`

Send an HTTP response to the client based on the provided response content.

### `void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);`

Parse a raw HTTP request and populate the provided `HttpRequest` structure.

## Getting Started

1. Include the library in your C project.
2. Initialize the server using `setupServer`.
3. Configure the server using `setPort`.
4. Handle incoming requests using `getRequest`.
5. Send response base on your own needs `sendResponse`.
6. Process and generate responses as needed.
7. Clean up resources with `cleanupServer` when done.

## Example

```c
#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define REQUEST_MAX_LENGTH 1000

void displayRequest(HttpRequest* request);

int main(int argc, char *argv[])
{
    // raw request is read into this
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    // create structure to parse raw data into
    HttpRequest request;
    createRequest(&request, 100, 1000);

    // create structure where all server data are stored and create server
    HttpConfig config;
    createServer(&config, 8080);

    // variable for storing count of bytes read
    size_t requestLength;

    // read request into buffer
    requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    // parse raw request into request structure
    parseRequest(requestBuffer, requestLength, &request);

    // this is not part of library
    // example of displaying parsed request
    displayRequest(&request);
    
    // generate response
    // you will be able to use HttpResponse structure with serializeResponse() function to create response easily, not implemented for now.
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";

    // send raw response to client
    sendResponse(&config, response, strlen(response));

    // clean-up
    deleteServer(&config);
    deleteRequest(&request);
    free(requestBuffer);
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
```  
### logs it gave me:  
```
Server listening on port 8080
Connection accepted from 127.0.0.1:35488
Successfully recieved request.
METHOD:  GET
PATH:    /
VERSION: HTTP/1.1
key Host: value: localhost:8080
key User-Agent: value: Mozilla/5.0(X11;Linuxx86_64;rv:109.0)Gecko/20100101Firefox/116.0
key Accept: value: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
key Accept-Language: value: cs,sk;q=0.8,en-US;q=0.5,en;q=0.3
key Accept-Encoding: value: gzip,deflate,br
key Connection: value: keep-alive
key Upgrade-Insecure-Requests: value: 1
key Sec-Fetch-Dest: value: document
key Sec-Fetch-Mode: value: navigate
key Sec-Fetch-Site: value: same-origin
body: 
```

## License
This project is licensed under GPL. Read more in LICENSE file.
