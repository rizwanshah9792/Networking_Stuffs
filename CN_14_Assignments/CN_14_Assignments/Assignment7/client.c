#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 8080

#pragma pack(push, 1)
typedef struct {
    char operation[10];
    double num1;
    double num2;
} CalculatorRequest;
#pragma pack(pop)

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serverAddr;
    CalculatorRequest req;
    double result;
    socklen_t addr_len = sizeof(serverAddr);

    if (argc < 2) {
        printf("Usage: %s <Server_IP_Address>\n", argv[0]);
        return 1;
    }

    // 1. Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // 3. Set timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
    }

    while (1) {
        memset(&req, 0, sizeof(req));  // clear struct each loop

        printf("\n--- Scientific UDP Calculator ---\n");
        printf("Operations: add, sub, mul, div, sin, cos, tan, sqrt, pow\n");
        printf("Enter operation (or 'exit'): ");

        // limit to 9 chars to avoid overflow
        if (scanf("%9s", req.operation) != 1) {
            printf("Input error.\n");
            break;
        }

        if (strcmp(req.operation, "exit") == 0)
            break;

        // One-arg operations
        if (strcmp(req.operation, "sin") == 0 ||
            strcmp(req.operation, "cos") == 0 ||
            strcmp(req.operation, "tan") == 0 ||
            strcmp(req.operation, "sqrt") == 0) {

            printf("Enter number: ");
            if (scanf("%lf", &req.num1) != 1) {
                printf("Input error.\n");
                break;
            }
            req.num2 = 0.0;

        } else {  // Two-arg operations
            printf("Enter first number: ");
            if (scanf("%lf", &req.num1) != 1) {
                printf("Input error.\n");
                break;
            }

            printf("Enter second number: ");
            if (scanf("%lf", &req.num2) != 1) {
                printf("Input error.\n");
                break;
            }
        }

        // 4. Send request
        sendto(sockfd, &req, sizeof(req), 0,
               (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

        // 5. Receive response
        int n = recvfrom(sockfd, &result, sizeof(result), 0,
                         (struct sockaddr *)&serverAddr, &addr_len);

        if (n < 0) {
            printf("\n[!] Error: Request timed out, Packet lost or server down.\n");
        } else {
            printf("\nServer Response: %.4f\n", result);
        }
    }

    close(sockfd);
    return 0;
}
