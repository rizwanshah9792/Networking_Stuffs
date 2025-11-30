#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8888
#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048

// Global variables
int clients[MAX_CLIENTS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void write_log(char *msg) {
    FILE *fp = fopen("log.txt", "a");
    if (fp == NULL) return;

    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0'; 

    fprintf(fp, "[%s] %s\n", date, msg);
    fclose(fp);
}

void send_to_all(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0 && clients[i] != sender_fd) {
            if (send(clients[i], msg, strlen(msg), 0) < 0) {
                perror("Sending failure");
            }
        }
    }
    pthread_mutex_unlock(&lock);
}

void *client_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    char name[32];
    int n;

    // 1. Get Name
    if (recv(sock, name, 32, 0) <= 0) {
        close(sock);
        return NULL;
    }
    
    // Create a safe buffer for the combined message
    // We give it extra padding (BUFFER_SIZE + 100) to be safe
    char formatted_msg[BUFFER_SIZE + 100];
    
    snprintf(formatted_msg, sizeof(formatted_msg), "--- %s has joined the chat ---", name);
    printf("%s\n", formatted_msg);
    send_to_all(formatted_msg, sock);
    write_log(formatted_msg);

    // 2. Receive Loop
    // Note: We recv (BUFFER_SIZE - 1) to ensure we have space for the \0
    while ((n = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[n] = '\0'; 
        
        // Fix: Use snprintf to prevent overflow
        snprintf(formatted_msg, sizeof(formatted_msg), "%s: %s", name, buffer);
        
        printf("%s\n", formatted_msg); 
        write_log(formatted_msg);      
        send_to_all(formatted_msg, sock); 
    }

    // 3. Cleanup
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == sock) {
            clients[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    
    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    memset(clients, 0, sizeof(clients));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    listen(server_fd, 10);

    printf("Chat Server Started on Port %d\nWaiting for connections...\n", PORT);

    addr_size = sizeof(client_addr);
    while ((new_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size))) {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == 0) {
                clients[i] = new_sock;
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        if (pthread_create(&thread_id, NULL, client_handler, (void*)&new_sock) < 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    return 0;
}
