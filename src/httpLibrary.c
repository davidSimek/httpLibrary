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

int initWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

// -1 == Couldn't initialize winsock
// -2 == Couldn't create socket
// -3 == Couldn't bind socket
// -4 == Couldn't listen on socket
int setupServer(HttpConfig* config) {
    if (initWinsock() != 0) {
        perror("Couldn't initialize windock.");
        return -1;
    }

    config->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (config->serverSocket == INVALID_SOCKET) {
        perror("Couldn't create socket.");
        WSACleanup();
        return -2;
    }

    // Set up serverAddress and other configuration...
    config->serverAddress.sin_family = AF_INET;
    config->serverAddress.sin_addr.s_addr = INADDR_ANY;
    config->serverAddress.sin_port = htons(config->port);

    if (bind(config->serverSocket, (struct sockaddr*)&config->serverAddress, sizeof(config->serverAddress)) == SOCKET_ERROR) {
        perror("Couldn't bind socket.");
        closesocket(config->serverSocket);
        WSACleanup();
        return -3;
    }

    if (listen(config->serverSocket, config->maxConnections) == SOCKET_ERROR) {
        closesocket(config->serverSocket);
        perror("Couldn't listen on socket");
        WSACleanup();
        return -4;
    }

    printf("Server listening on port %d\n", config->port);
    return 0;
}

void cleanupServer(HttpConfig* config) {
    closesocket(config->serverSocket);
    WSACleanup();
}

void setPort(HttpConfig* config, int port) {
    config->port = port;
}

//  n == requestLength
// -1 == couldn't read from client
int getRequest(HttpConfig* config, char* request, size_t requestMaxLength) {
    int bytesRead = recv(config->clientSocket, request, requestMaxLength, 0);
    if (bytesRead == SOCKET_ERROR) {
        perror("Couldn't read from client.");
        return -1;
    }

    return bytesRead;
}

void parseRequest(const HttpRequest* request, char* output, size_t bufferSize) {
    // Implementation for parsing the request...
}

// out
void serializeResponse(char* input, const HttpResponse* response, size_t bufferSize) {
    // Implementation for serializing the response...
}

void sendResponse(HttpConfig* config, char* response, size_t responseLength) {
    send(config->clientSocket, response, responseLength, 0);
}

#endif

// these should be same for linux and windows
void parseRequest(const HttpRequest* request, char* output, size_t bufferSize);
void serializeResponse(char* input, const HttpResponse* response, size_t bufferSize);

