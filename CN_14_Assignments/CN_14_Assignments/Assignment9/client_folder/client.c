#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file(int socket, char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("[-] Error reading file to upload");
        exit(1);
    }
    char data[BUFFER_SIZE] = {0};
    while (fread(data, 1, BUFFER_SIZE, fp) > 0) {
        if (send(socket, data, sizeof(data), 0) == -1) {
            perror("[-] Error sending file");
            exit(1);
        }
        bzero(data, BUFFER_SIZE);
    }
    fclose(fp);
}

void receive_file(int socket, char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("[-] Error creating file for download");
        exit(1);
    }
    char buffer[BUFFER_SIZE];
    int n;
    // Simple read loop
    while ((n = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, n, fp);
        if (n < BUFFER_SIZE) break;
    }
    fclose(fp);
}

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    clock_t start, end;
    double cpu_time_used;

    if (argc < 2) {
        printf("Usage: %s <Server IP Address>\n", argv[0]);
        return -1;
    }

    // 1. Create Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n[-] Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and connect
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\n[-] Invalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n[-] Connection Failed \n");
        return -1;
    }
    printf("[+] Connected to Server.\n");

    // --- TASK 1: Download File from Server ---
    printf("[*] Downloading 'server_data.txt' as 'downloaded_from_server.txt'...\n");
    start = clock();
    
    receive_file(sock, "downloaded_from_server.txt");
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[+] Download Complete! Time: %f sec\n", cpu_time_used);

    sleep(1); // Brief pause to sync with server

    // --- TASK 2: Upload File to Server ---
    printf("[*] Uploading 'client_data.txt' to Server...\n");
    start = clock();
    
    send_file(sock, "client_data.txt");
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("[+] Upload Complete! Time: %f sec\n", cpu_time_used);

    close(sock);
    return 0;
}
