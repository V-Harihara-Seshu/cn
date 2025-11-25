#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9876
#define BUF_SIZE 1024

void runServer();
void runClient();

int main() {
    int choice;

    printf("1. Run as Server\n");
    printf("2. Run as Client\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); // clear newline

    if (choice == 1) {
        runServer();
    }
    else if (choice == 2) {
        runClient();
    }
    else {
        printf("Invalid choice!\n");
    }

    return 0;
}

// -----------------------------------------
// SERVER CODE
// -----------------------------------------
void runServer() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[BUF_SIZE], sendBuffer[BUF_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running... waiting for client message...\n");

    // Receive message
    int n = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                     (struct sockaddr*)&clientAddr, &clientAddrLen);
    buffer[n] = '\0';

    printf("RECEIVED: %s\n", buffer);

    // Send reply
    printf("Enter the message to send to client: ");
    fgets(sendBuffer, BUF_SIZE, stdin);

    sendto(sockfd, sendBuffer, strlen(sendBuffer), 0,
           (struct sockaddr*)&clientAddr, clientAddrLen);

    printf("Message sent to client.\n");

    close(sockfd);
}

// -----------------------------------------
// CLIENT CODE
// -----------------------------------------
void runClient() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
    char buffer[BUF_SIZE];
    char *message = "Hello Server";

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost

    // Send message
    sendto(sockfd, message, strlen(message), 0,
           (struct sockaddr*)&serverAddr, serverAddrLen);

    printf("Message sent to server.\n");

    // Receive reply
    int n = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                     (struct sockaddr*)&serverAddr, &serverAddrLen);
    buffer[n] = '\0';

    printf("FROM SERVER: %s\n", buffer);

    close(sockfd);
}
