/*
 * Simple TCP Client for Testing the Analyzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *server_ip = "127.0.0.1";
    
    if (argc > 1) {
        server_ip = argv[1];
    }
    
    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Connected to server %s:%d\n", server_ip, PORT);
    printf("Type messages to send (or 'quit' to exit):\n\n");
    
    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        
        if (strncmp(buffer, "quit", 4) == 0) {
            break;
        }
        
        // Send message
        send(sock_fd, buffer, strlen(buffer), 0);
        
        // Receive response
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Server: %s\n", buffer);
        }
    }
    
    close(sock_fd);
    printf("Disconnected\n");
    return 0;
}
