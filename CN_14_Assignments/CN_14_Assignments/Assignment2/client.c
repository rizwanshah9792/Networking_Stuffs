// t_client.c (Modified for Assignment 2)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7891

int main(int argc, char const *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(1);
    }
    const char *server_ip = argv[1];

    int clientSocket;
    char buffer[1024];
    char user_input[1024];
    struct sockaddr_in serverAddr;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Read the initial welcome message
    recv(clientSocket, buffer, 1024, 0);
    printf("Server: %s\n", buffer);

    printf("Commands: \n");
    printf("  BUY <fruitname> <quantity>  (e.g., BUY apple 5)\n");
    printf("  EXIT\n\n");

    // --- User Interaction Loop ---
    while(1) {
        printf("> ");
        fgets(user_input, 1024, stdin);
        user_input[strcspn(user_input, "\n")] = 0; // Remove trailing newline

        if(strlen(user_input) == 0) continue; // Skip empty input

        send(clientSocket, user_input, strlen(user_input), 0);

        if(strncmp(user_input, "EXIT", 4) == 0) {
            printf("Disconnecting...\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, 1024, 0);
        printf("Server: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
