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

### `int setupServer(HttpConfig* config);`

Initialize and set up the HTTP server with the provided configuration.

### `void cleanupServer(HttpConfig* config);`

Clean up and release resources associated with the HTTP server.

### `void setPort(HttpConfig* config, int port);`

Set the port on which the server will listen for incoming connections.

### `int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);`

Receive an HTTP request from the client and store it in the provided buffer.

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

int main(int argc, char *argv[])
{
    // allocate space for request struct
    HttpRequest* request = (HttpRequest*)malloc(sizeof(HttpRequest));
    // allocate space for body of request
    request->body = (char*)malloc(BODY_LENGTH);
    // allocate buffer to request into
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    // setup all variables and "run" server
    HttpConfig config;
    setPort(&config, 8080);
    setupServer(&config);

    // get request
    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);

    // parse request
    parseRequest(requestBuffer, requestLength, request);

    //---------------
    // DO YOUR STUFF
    //---------------
    
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";

    // send response
    sendResponse(&config, response, strlen(response));

    // "stop" server
    cleanupServer(&config);

    // for now you are responsible for all allocations
    // my plan is to mange this, trying to think of the best way to execute this
    free(request->body);
    free(request);
    free(requestBuffer);
    return 0;
}
```

## License
This project is licensed under GPL. Read more in LICENSE file.
