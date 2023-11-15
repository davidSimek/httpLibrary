#include "httpLibrary.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#ifndef WINDOWS

// -1 == couldn't create socket
// -2 == couldn't bind socket
// -3 == cannot listen for connections
int createServer(HttpConfig* config, int port) {
    config->port = port;
    config->clientAddressLength = sizeof(config->clientAddress);

    if ((config->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket couldn't be created.");
        return -1;
    }

    config->serverAddress.sin_family = AF_INET;
    config->serverAddress.sin_addr.s_addr = INADDR_ANY;
    config->serverAddress.sin_port = htons(config->port);

    if (bind(config->serverSocket, (struct sockaddr*)&config->serverAddress, sizeof(config->serverAddress)) == -1) {
        perror("Socket couldn't bind.");
        return -2;
    }

    if (listen(config->serverSocket, config->maxConntections) == -1) {
        perror("Cannot listen for connections.");
        return -3;
    }

    printf("Server listening on port %d\n", config->port);
    return 1;
}
void deleteServer(HttpConfig* config) {
    close(config->serverSocket);
}

//   n == requestLenght
//  -1 == Couldn't accept connection.
//  -2 == Couldn't read from client.
int getRequest(HttpConfig* config, char* request, size_t requestMaxLength) {
    if ((config->clientSocket = accept(config->serverSocket, (struct sockaddr*)&config->clientAddress, &config->clientAddressLength)) == -1) {
        close(config->clientSocket);
        perror("Couldn't accept connection");
        return -1;
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(config->clientAddress.sin_addr), ntohs(config->clientAddress.sin_port));
   
    size_t requestLength = recv(config->clientSocket, request, requestMaxLength - 1, 0);
    if (requestLength == -1) {
        perror("Couldn't read from client.");
        return -2;
    }

    request[requestLength] = '\0';
    puts("Successfully recieved request.");
    return requestLength;
}
void sendResponse(HttpConfig* config, char* response, size_t responseLength) {
    send(config->clientSocket, response, responseLength, 0); 
    close(config->clientSocket);
}

#else

// -1 == couldn't create socket
// -2 == couldn't bind socket
// -3 == cannot listen for connections
int createServer(HttpConfig* config, int port) {
    config->port = port;
    config->clientAddressLength = sizeof(config->clientAddress);

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return -1;
    }

    if ((config->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Socket couldn't be created.");
        WSACleanup();
        return -1;
    }

    config->serverAddress.sin_family = AF_INET;
    config->serverAddress.sin_addr.s_addr = INADDR_ANY;
    config->serverAddress.sin_port = htons(config->port);

    if (bind(config->serverSocket, (struct sockaddr*)&config->serverAddress, sizeof(config->serverAddress)) == SOCKET_ERROR) {
        perror("Socket couldn't bind.");
        closesocket(config->serverSocket);
        WSACleanup();
        return -2;
    }

    if (listen(config->serverSocket, config->maxConnections) == SOCKET_ERROR) {
        perror("Cannot listen for connections.");
        closesocket(config->serverSocket);
        WSACleanup();
        return -3;
    }

    printf("Server listening on port %d\n", config->port);
    return 1;
}

void deleteServer(HttpConfig* config) {
    closesocket(config->serverSocket);
    WSACleanup();
}

//   n == requestLength
//  -1 == Couldn't accept connection.
//  -2 == Couldn't read from client.
int getRequest(HttpConfig* config, char* request, size_t requestMaxLength) {
    if ((config->clientSocket = accept(config->serverSocket, (struct sockaddr*)&config->clientAddress, &config->clientAddressLength)) == INVALID_SOCKET) {
        closesocket(config->clientSocket);
        perror("Couldn't accept connection");
        return -1;
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(config->clientAddress.sin_addr), ntohs(config->clientAddress.sin_port));
   
    size_t requestLength = recv(config->clientSocket, request, requestMaxLength - 1, 0);
    if (requestLength == SOCKET_ERROR) {
        perror("Couldn't read from client.");
        return -2;
    }

    request[requestLength] = '\0';
    puts("Successfully received request.");
    return requestLength;
}

void sendResponse(HttpConfig* config, char* response, size_t responseLength) {
    send(config->clientSocket, response, responseLength, 0); 
    closesocket(config->clientSocket);
}

#endif

// these should be same for linux and windows
void parseRequest(char* rawRequest, size_t rawRequestLength, HttpRequest* request) {
    // stage:
    //      0 -> method,
    //      1 -> path
    //      2 -> version
    //      3 -> haeders
    //      4 -> body
    int stage = 0;
    // index starting from 0 in each stage
    int stageIndex = 0;
    // switching key/value for headers
    bool isKey = true;
    // to find on which char I am in headers stage
    int charIndex = 0;
    for (int i = 0; i < rawRequestLength; i++) {
        char currentChar = rawRequest[i];

        if (stage == 0) {
            if (currentChar == ' ') {
                request->method[stageIndex] = '\0';
                stage = 1;
                stageIndex = 0;
                continue;
            }
            request->method[stageIndex] = currentChar;
            stageIndex++;
        }

        else if (stage == 1) {
            if (currentChar == ' ') {
                request->path[stageIndex] = '\0';
                stage = 2;
                stageIndex = 0;
                continue;
            }
            request->path[stageIndex] = currentChar;
            stageIndex++;
        }

        else if (stage == 2) {
            if (currentChar == '\n') {
                request->version[stageIndex] = '\0';
                stage = 3;
                stageIndex = 0;
                continue;
            }
            request->version[stageIndex] = currentChar;
            stageIndex++;
        }

        else if (stage == 3) {
            if (stageIndex >= request->headerMapSize) {
                stage = 4;
                stageIndex = 0;
                continue;
            }
            // this checks for end of headers section
            if (currentChar == '\r' && rawRequest[i + 1] == '\n' && rawRequest[i + 2] == '\r' && rawRequest[i + 3] == '\n') {
                stage = 4;
                stageIndex = 0;
                continue;
            }
            if (currentChar == '\n') {
                stageIndex++;
                isKey = true;
                request->headerMap[stageIndex].key[charIndex] = '\0';
                charIndex=0;
                continue;
            }

            if (currentChar == ' ') {
                continue;
            }

            if (currentChar == ':' && isKey) {
                request->headerMap[stageIndex].value[charIndex] = '\0';
                charIndex = 0;
                isKey = false;
                continue;
            }

            if (isKey) {
                request->headerMap[stageIndex].key[charIndex] = currentChar;
                charIndex++;
                continue;
            }
            request->headerMap[stageIndex].value[charIndex] = currentChar;
            charIndex++;
        }
        else if (stage == 4) {
            if (stageIndex == BODY_LENGTH - 1 || currentChar == '\0') {
                request->body[stageIndex] = '\0';
                return;
            }
            request->body[stageIndex] = currentChar;
            stageIndex++;
        }
    }
}
void createRequest(HttpRequest* request, size_t maxHeaderCount, size_t maxSizeOfBody) {
    request->headerMapSize = maxHeaderCount;

    request->headerMap = (Header*)malloc(sizeof(Header) * (request->headerMapSize + 1));

    request->body = (char*)malloc(maxSizeOfBody);
}

void deleteRequest(HttpRequest *request) {
    free(request->headerMap);
    free(request->body);
}

void serializeResponse();

