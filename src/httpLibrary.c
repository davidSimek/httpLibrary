#include "httpLibrary.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

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
void parseRequest(const HttpRequest* request, char* output, size_t bufferSize);

// out
void serializeResponse(char* input, const HttpResponse* response, size_t bufferSize);
void sendResponse(HttpConfig* config, char* response, size_t responseLength) {
    send(config->clientSocket, response, responseLength, 0); 
    close(config->clientSocket);
}
