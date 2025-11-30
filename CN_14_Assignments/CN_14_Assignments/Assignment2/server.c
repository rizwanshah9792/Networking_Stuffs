// t_server.c (Modified for Assignment 2)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

#define PORT 7891
#define MAX_FRUITS 5
#define MAX_CUSTOMERS 50
#define SHM_KEY 0x1234 // Unique key for shared memory

// --- Data Structures to be stored in Shared Memory ---
struct Fruit {
    char name[20];
    int quantity;
    time_t last_sold_timestamp;
};

struct Customer {
    char ip_address[16];
    int port;
};

// A single struct to hold all our shared data
struct SharedData {
    sem_t mutex; // Semaphore for locking
    int unique_customer_count;
    struct Fruit inventory[MAX_FRUITS];
    struct Customer customers[MAX_CUSTOMERS];
};

struct SharedData *shared_data;
int shm_id;

// --- Handles all logic for a single connected client ---
void handle_client(int client_socket, struct sockaddr_in client_addr) {
    char buffer[1024] = {0};
    char response[1024] = {0};
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

    printf("Connection accepted from %s:%d\n", client_ip, client_port);
    snprintf(response, sizeof(response), "Welcome! Total unique customers so far: %d", shared_data->unique_customer_count);
    send(client_socket, response, strlen(response), 0);

    // Loop to process multiple requests from this client
    while (read(client_socket, buffer, 1024) > 0) {
        char command[10], fruit_name[20];
        int quantity;
        sscanf(buffer, "%s %s %d", command, fruit_name, &quantity);

        sem_wait(&shared_data->mutex); // Lock shared memory

        if (strcmp(command, "BUY") == 0) {
            int found = 0;
            for (int i = 0; i < MAX_FRUITS; i++) {
                if (strcmp(shared_data->inventory[i].name, fruit_name) == 0) {
                    found = 1;
                    if (shared_data->inventory[i].quantity >= quantity) {
                        shared_data->inventory[i].quantity -= quantity;
                        shared_data->inventory[i].last_sold_timestamp = time(NULL);

                        int is_new_customer = 1;
                        for(int j = 0; j < shared_data->unique_customer_count; j++) {
                            if(strcmp(shared_data->customers[j].ip_address, client_ip) == 0 && shared_data->customers[j].port == client_port) {
                                is_new_customer = 0;
                                break;
                            }
                        }
                        if(is_new_customer) {
                            strcpy(shared_data->customers[shared_data->unique_customer_count].ip_address, client_ip);
                            shared_data->customers[shared_data->unique_customer_count].port = client_port;
                            shared_data->unique_customer_count++;
                        }
                        sprintf(response, "SUCCESS: Purchase complete. Total unique customers: %d", shared_data->unique_customer_count);
                    } else {
                        sprintf(response, "REGRET: Sorry, we only have %d of %s.", shared_data->inventory[i].quantity, fruit_name);
                    }
                    break;
                }
            }
            if (!found) sprintf(response, "REGRET: Fruit '%s' not found.", fruit_name);
        } else {
            strcpy(response, "ERROR: Invalid command. Use 'BUY <fruit> <qty>'.");
        }

        sem_post(&shared_data->mutex); // Unlock shared memory

        send(client_socket, response, strlen(response), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    printf("Client %s:%d disconnected.\n", client_ip, client_port);
    close(client_socket);
    exit(0);
}

// --- Cleans up shared memory on server exit (Ctrl+C) ---
void cleanup(int sig) {
    printf("\nServer shutting down. Cleaning up shared memory...\n");
    shmdt(shared_data);
    shmctl(shm_id, IPC_RMID, NULL);
    exit(0);
}

int main() {
    // --- Shared Memory Setup ---
    shm_id = shmget(SHM_KEY, sizeof(struct SharedData), IPC_CREAT | 0666);
    shared_data = (struct SharedData *)shmat(shm_id, NULL, 0);
    
    // Check if memory is new, then initialize it
    if(shared_data->inventory[0].quantity == 0 && strcmp(shared_data->inventory[0].name, "") == 0) {
        printf("Initializing shared data for the first time.\n");
        sem_init(&shared_data->mutex, 1, 1);
        shared_data->unique_customer_count = 0;
        strcpy(shared_data->inventory[0].name, "apple"); shared_data->inventory[0].quantity = 100;
        strcpy(shared_data->inventory[1].name, "banana"); shared_data->inventory[1].quantity = 150;
        strcpy(shared_data->inventory[2].name, "orange"); shared_data->inventory[2].quantity = 80;
    }

    // --- Socket Setup (from your original code) ---
    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr, clientStorage;
    socklen_t addr_size;

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Changed to INADDR_ANY for robustness
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (listen(welcomeSocket, 5) == 0)
        printf("Listening on port %d...\n", PORT);
    else
        printf("Error\n");

    signal(SIGINT, cleanup); // Register cleanup function for Ctrl+C

    // --- Main Server Loop ---
    while (1) {
        addr_size = sizeof clientStorage;
        newSocket = accept(welcomeSocket, (struct sockaddr *) &clientStorage, &addr_size);

        if (fork() == 0) { // Child process
            close(welcomeSocket);
            handle_client(newSocket, clientStorage);
        } else { // Parent process
            close(newSocket);
            waitpid(-1, NULL, WNOHANG); // Clean up zombie child processes
        }
    }
    return 0;
}
