# My HTTP Library

super simple http implementation with parser and serializer

## Table of Contents
- [Introduction](#introduction)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Currently working function](#currently woking functions)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Introduction

I've often desired to experiment with server/client architecture, but faced the challenge of either repeatedly implementing HTTP functionality or relying on feature-rich libraries with more than necessary components. In response, I developed this solution. It offers a straightforward interface with heightened usability. With this tool, creating a basic HTTP server requires fewer than 10 lines of code, providing comprehensive control over the server's operations.

## Getting Started

### Prerequisites

GNU/Linux or Widnows system (Windows should work, but it might not work as well)

## Usage

You can't go simpler.  
```c
// #define WINDOWS if building for windows
// you can do this in gcc like this: gcc -DWINDOWS=1 ...
#include "httpLibrary.h"

#include <stdio.h>
#include <string.h>

#define REQUEST_MAX_LENGTH 1000

int main(int argc, char *argv[])
{
    HttpConfig config;
    setPort(&config, 8080);
    setupServer(&config);
    char* requestBuffer = (char*)malloc(REQUEST_MAX_LENGTH + 1);

    size_t requestLength = getRequest(&config, requestBuffer, REQUEST_MAX_LENGTH);
    printf("request is :\n%s\n", requestBuffer);

    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
    sendResponse(&config, response, strlen(response));
    cleanupServer(&config);

    free(requestBuffer);
    return 0;
}
```

## Currently woking functions

`int setupServer(HttpConfig* config);`  
called once to initialize server  
`void cleanupServer(HttpConfig* config);`  
called once you want to exit process  
`void setPort(HttpConfig* config, int port);`  
set port you want your web exposed on  
`int getRequest(HttpConfig* config, char* request, size_t requestMaxLength);`  
access recieved request (blocking), returns lenght of that request  
sends response  
`void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request);`  
parses raw http request into HttpRequest struct  

## Troubleshooting

As this repo contains build script, you can compile with -g flag. Run `./build.sh` and it will guide you.
There are also logs, which will be optional in future. I just have it on all the time for debug purposes.

## License

This project utilizes GNU GENERAL PUBLIC LICENSE. More info in LICENSE file.
