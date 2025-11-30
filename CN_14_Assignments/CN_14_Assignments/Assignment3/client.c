#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void display_menu() {
    printf("\n========================================\n");
    printf("   FRUIT STORE CLIENT MENU\n");
    printf("========================================\n");
    printf("1. View Stock\n");
    printf("2. Buy Fruit\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

void parse_response(char *response) {
    char type[20], message[BUFFER_SIZE];
    int unique_customers;
    
    // Parse response format: TYPE|MESSAGE|CUSTOMER_COUNT
    if (sscanf(response, "%[^|]|%[^|]|%d", type, message, &unique_customers) >= 2) {
        printf("\n========================================\n");
        
        if (strcmp(type, "SUCCESS") == 0) {
            printf("✓ SUCCESS\n");
            printf("----------------------------------------\n");
            printf("%s\n", message);
        } else if (strcmp(type, "REGRET") == 0) {
            printf("✗ REGRET\n");
            printf("----------------------------------------\n");
            printf("%s\n", message);
        } else if (strcmp(type, "STOCK") == 0) {
            printf("CURRENT STOCK INFORMATION\n");
            printf("----------------------------------------\n");
            printf("%s\n", message);
        } else if (strcmp(type, "ERROR") == 0) {
            printf("✗ ERROR\n");
            printf("----------------------------------------\n");
            printf("%s\n", message);
        }
        
        if (unique_customers > 0) {
            printf("----------------------------------------\n");
            printf("Total Unique Customers: %d\n", unique_customers);
        }
        
        printf("========================================\n");
    } else {
        printf("\nServer Response: %s\n", response);
    }
}

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char server_ip[20] = "127.0.0.1";
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    
    // Allow custom server IP from command line
    if (argc > 1) {
        strcpy(server_ip, argv[1]);
    }
    
    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    
    printf("=================================================\n");
    printf("   CONNECTED TO FRUIT STORE SERVER\n");
    printf("   Server: %s:%d\n", server_ip, PORT);
    printf("=================================================\n");
    
    int choice;
    char fruit_name[50];
    int quantity;
    
    while (1) {
        display_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline
        
        memset(buffer, 0, BUFFER_SIZE);
        
        switch (choice) {
            case 1:
                // View stock
                strcpy(buffer, "STOCK");
                break;
                
            case 2:
                // Buy fruit
                printf("\nEnter fruit name: ");
                scanf("%s", fruit_name);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                getchar();
                
                sprintf(buffer, "BUY %s %d", fruit_name, quantity);
                break;
                
            case 3:
                printf("\nThank you for visiting our store!\n");
                close(sockfd);
                exit(0);
                
            default:
                printf("\nInvalid choice! Please try again.\n");
                continue;
        }
        
        // Send request to server
        sendto(sockfd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&server_addr, addr_len);
        
        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                        (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';
        
        // Display response
        parse_response(buffer);
    }
    
    close(sockfd);
    return 0;
}
