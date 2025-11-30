#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BUFFER_SIZE 2048

int sock;
char name[32];

// Thread 1: Listens for incoming messages from server
void *receive_msg_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    int n;
    while (1) {
        n = recv(sock, buffer, BUFFER_SIZE, 0);
        if (n > 0) {
            buffer[n] = '\0';
            printf("%s\n", buffer); // Print message from others
        } else {
            // If recv returns 0, server disconnected
            printf("Server disconnected.\n");
            exit(0);
        }
    }
}

// Thread 2: Handles user typing and sending
void *send_msg_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        
        // Remove the "Enter" key newline character
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            break;
        } else {
            send(sock, buffer, strlen(buffer), 0);
        }
    }
    close(sock);
    exit(0);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    pthread_t send_thread, recv_thread;

    if (argc < 2) {
        printf("Usage: %s <Server_IP>\n", argv[0]);
        return 1;
    }

    printf("Enter your name: ");
    fgets(name, 32, stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline

    // Create Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Connect
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }

    // Send the name first so server knows who we are
    send(sock, name, 32, 0);

    printf("--- Welcome to the Group Chat %s! ---\n", name);

    // Create two threads: One for talking, one for listening
    pthread_create(&send_thread, NULL, send_msg_handler, NULL);
    pthread_create(&recv_thread, NULL, receive_msg_handler, NULL);

    // Wait for threads to finish (they practically run forever)
    pthread_join(send_thread, NULL);
    pthread_join(recv_thread, NULL);

    return 0;
}
