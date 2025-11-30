#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_FRUITS 10
#define MAX_CLIENTS 50

// Structure to store fruit information
struct Fruit {
    char name[50];
    int quantity;
    char last_sold[30];
};

// Structure to store client information
struct Client {
    char ip[20];
    int port;
};

struct Fruit fruits[MAX_FRUITS];
struct Client clients[MAX_CLIENTS];
int fruit_count = 0;
int client_count = 0;

// Function to get current timestamp
void get_timestamp(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d", 
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);
}

// Function to initialize fruits
void initialize_fruits() {
    strcpy(fruits[0].name, "Apple");
    fruits[0].quantity = 100;
    strcpy(fruits[0].last_sold, "Not sold yet");
    
    strcpy(fruits[1].name, "Banana");
    fruits[1].quantity = 150;
    strcpy(fruits[1].last_sold, "Not sold yet");
    
    strcpy(fruits[2].name, "Orange");
    fruits[2].quantity = 80;
    strcpy(fruits[2].last_sold, "Not sold yet");
    
    strcpy(fruits[3].name, "Mango");
    fruits[3].quantity = 60;
    strcpy(fruits[3].last_sold, "Not sold yet");
    
    strcpy(fruits[4].name, "Grapes");
    fruits[4].quantity = 120;
    strcpy(fruits[4].last_sold, "Not sold yet");
    
    fruit_count = 5;
}

// Function to check if client already exists
int is_client_exists(char *ip, int port) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].ip, ip) == 0 && clients[i].port == port) {
            return 1;
        }
    }
    return 0;
}

// Function to add new client
void add_client(char *ip, int port) {
    if (!is_client_exists(ip, port) && client_count < MAX_CLIENTS) {
        strcpy(clients[client_count].ip, ip);
        clients[client_count].port = port;
        client_count++;
        printf("\n[NEW CLIENT] Added: %s:%d\n", ip, port);
    }
}

// Function to display all clients
void display_clients() {
    printf("\n========== CLIENT TRANSACTION HISTORY ==========\n");
    printf("Total Unique Customers: %d\n", client_count);
    printf("-----------------------------------------------\n");
    for (int i = 0; i < client_count; i++) {
        printf("%d. IP: %s, Port: %d\n", i + 1, clients[i].ip, clients[i].port);
    }
    printf("================================================\n\n");
}

// Function to process purchase request
void process_purchase(char *fruit_name, int quantity, char *response) {
    int found = 0;
    char timestamp[30];
    
    for (int i = 0; i < fruit_count; i++) {
        if (strcasecmp(fruits[i].name, fruit_name) == 0) {
            found = 1;
            
            if (fruits[i].quantity >= quantity) {
                // Sufficient quantity available
                fruits[i].quantity -= quantity;
                get_timestamp(timestamp);
                strcpy(fruits[i].last_sold, timestamp);
                
                sprintf(response, "SUCCESS|Purchase successful! %d %s(s) bought. Remaining stock: %d|%d", 
                        quantity, fruits[i].name, fruits[i].quantity, client_count);
                
                printf("[TRANSACTION] Sold %d %s(s). Remaining: %d\n", 
                       quantity, fruits[i].name, fruits[i].quantity);
            } else {
                // Insufficient quantity
                sprintf(response, "REGRET|Sorry! Only %d %s(s) available. You requested %d.|%d", 
                        fruits[i].quantity, fruits[i].name, quantity, client_count);
                
                printf("[REGRET] Insufficient stock for %s. Available: %d, Requested: %d\n", 
                       fruits[i].name, fruits[i].quantity, quantity);
            }
            break;
        }
    }
    
    if (!found) {
        sprintf(response, "ERROR|Fruit '%s' not found in our store.|%d", fruit_name, client_count);
    }
}

// Function to get stock information
void get_stock_info(char *response) {
    strcpy(response, "STOCK|");
    char temp[200];
    
    for (int i = 0; i < fruit_count; i++) {
        sprintf(temp, "%s: %d (Last sold: %s)\n", 
                fruits[i].name, fruits[i].quantity, fruits[i].last_sold);
        strcat(response, temp);
    }
    
    sprintf(temp, "Total unique customers: %d|%d", client_count, client_count);
    strcat(response, temp);
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Initialize fruits
    initialize_fruits();
    
    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("=================================================\n");
    printf("   UDP FRUIT STORE SERVER STARTED\n");
    printf("=================================================\n");
    printf("Server listening on port %d...\n\n", PORT);
    
    // Server loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        memset(response, 0, BUFFER_SIZE);
        
        // Receive message from client
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                        (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';
        
        char client_ip[20];
        strcpy(client_ip, inet_ntoa(client_addr.sin_addr));
        int client_port = ntohs(client_addr.sin_port);
        
        printf("\n[REQUEST] From %s:%d - %s\n", client_ip, client_port, buffer);
        
        // Parse the request
        char command[20], fruit_name[50];
        int quantity;
        
        if (sscanf(buffer, "%s", command) == 1) {
            if (strcmp(command, "STOCK") == 0) {
                // Send stock information
                get_stock_info(response);
            } else if (strcmp(command, "BUY") == 0) {
                // Process purchase
                if (sscanf(buffer, "BUY %s %d", fruit_name, &quantity) == 2) {
                    add_client(client_ip, client_port);
                    process_purchase(fruit_name, quantity, response);
                    display_clients();
                } else {
                    sprintf(response, "ERROR|Invalid format. Use: BUY <fruit_name> <quantity>|%d", client_count);
                }
            } else {
                sprintf(response, "ERROR|Unknown command. Use STOCK or BUY|%d", client_count);
            }
        }
        
        // Send response to client
        sendto(sockfd, response, strlen(response), 0,
               (const struct sockaddr *)&client_addr, addr_len);
        
        printf("[RESPONSE] Sent to %s:%d\n", client_ip, client_port);
    }
    
    close(sockfd);
    return 0;
}

