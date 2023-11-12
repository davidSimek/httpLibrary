# My HTTP Library

super simple http implementation with parser and serializer

## Table of Contents
- [Introduction](#introduction)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Introduction

I've often desired to explore server/client architecture, but faced the challenge of either repeatedly implementing HTTP functionality or relying on feature-rich libraries with more than necessary components. In response, I developed this solution. It offers a straightforward interface with heightened usability. With this tool, creating a basic HTTP server requires fewer than 10 lines of code, providing comprehensive control over the server's operations.

## Getting Started

### Prerequisites

GNU/Linux or Widnows system (Windows in not tested yet)

## Usage

You can't go simpler.  
```c
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

## Troubleshooting

## License
