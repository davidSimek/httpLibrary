#include "httpLibrary.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#ifndef WINDOWS
void setPort(HttpConfig* config, int port) {
    config->port = port;
}

// -1 == couldn't create socket
// -2 == couldn't bind socket
// -3 == cannot listen for connections
int setupServer(HttpConfig* config) {
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
void cleanupServer(HttpConfig* config) {
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


void setPort(HttpConfig* config, int port) {
    config->port = port;
}

// -1 == couldn't create socket
// -2 == couldn't bind socket
// -3 == cannot listen for connections
int setupServer(HttpConfig* config) {
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

void cleanupServer(HttpConfig* config) {
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
    int stageIndex;
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

        if (stage == 1) {
            if (currentChar == ' ') {
                request->path[stageIndex] = '\0';
                stage = 2;
                stageIndex = 0;
                continue;
            }
            request->path[stageIndex] = currentChar;
            stageIndex++;
        }

        if (stage == 2) {
            if (currentChar == '\n') {
                request->version[stageIndex] = '\0';
                stage = 3;
                stageIndex = 0;
                continue;
            }
            request->version[stageIndex] = currentChar;
            stageIndex++;
        }
    }
}
void serializeResponse();

