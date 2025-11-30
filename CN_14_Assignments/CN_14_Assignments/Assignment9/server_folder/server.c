#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Helper function to send a file
void send_file(int socket, char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("[-] Error in reading file.");
        exit(1);
    }

    char data[BUFFER_SIZE] = {0};
    while (fread(data, 1, BUFFER_SIZE, fp) > 0) {
        if (send(socket, data, sizeof(data), 0) == -1) {
            perror("[-] Error in sending file.");
            exit(1);
        }
        bzero(data, BUFFER_SIZE); // Clear buffer
    }
    // Send a specific marker or close signal logic would go here for complex apps
    // For this simple assignment, we rely on flow control
    fclose(fp);
}

// Helper function to receive a file
void receive_file(int socket, char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("[-] Error in creating file.");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int n;
    // Simplified: We read until the socket is empty or connection closes
    // Note: In a real production app, we would send file size headers first.
    while ((n = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, n, fp);
        if (n < BUFFER_SIZE) break; // Simple break condition for small files
    }
    fclose(fp);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    clock_t start, end;
    double cpu_time_used;

    // 1. Create Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("[-] Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Bind and Listen
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[-] Bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("[-] Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[+] Server is running on port %d...\n", PORT);

    // 3. Accept Connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("[-] Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Client connected.\n");

    // --- TASK 1: Server Sends File (Client Downloads) ---
    printf("[*] Starting transfer: Sending 'server_data.txt' to Client...\n");
    start = clock();
    
    send_file(new_socket, "server_data.txt");
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[+] File Sent! Time taken: %f seconds\n", cpu_time_used);

    // Small delay to ensure network buffers clear between tasks
    sleep(1); 

    // --- TASK 2: Server Receives File (Client Uploads) ---
    // Note: For this simple logic, we might need a new connection or robust loop. 
    // To keep it simple for the assignment, we will just close and wait for the upload logic 
    // or assume the client sends immediately. 
    
    printf("[*] Waiting to receive 'uploaded_by_client.txt'...\n");
    start = clock();
    
    receive_file(new_socket, "uploaded_by_client.txt");
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[+] File Received! Time taken: %f seconds\n", cpu_time_used);

    close(new_socket);
    close(server_fd);
    return 0;
}
