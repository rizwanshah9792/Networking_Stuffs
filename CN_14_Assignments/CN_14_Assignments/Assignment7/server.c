#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

// Make sure client & server use exactly the same packed struct
#pragma pack(push, 1)
typedef struct {
    char operation[10];   // e.g. "add", "sin"
    double num1;
    double num2;
} CalculatorRequest;
#pragma pack(pop)

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_len = sizeof(clientAddr);
    CalculatorRequest req;
    double result;

    // 1. Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 3. Bind socket
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Scientific Calculator Server is running on UDP Port %d...\n", PORT);

    while (1) {
        memset(&req, 0, sizeof(req));  // avoid garbage in struct
        int n = recvfrom(sockfd, &req, sizeof(req), 0,
                         (struct sockaddr *)&clientAddr, &addr_len);

        if (n > 0) {
            printf("Received Request: %s, on %.2f and %.2f\n",
                   req.operation, req.num1, req.num2);

            // Always start from 0
            result = 0.0;

            // Do the operation
            if (strcmp(req.operation, "add") == 0) {
                result = req.num1 + req.num2;
            } else if (strcmp(req.operation, "sub") == 0) {
                result = req.num1 - req.num2;
            } else if (strcmp(req.operation, "mul") == 0) {
                result = req.num1 * req.num2;
            } else if (strcmp(req.operation, "div") == 0) {
                result = req.num1 / req.num2;
            } else if (strcmp(req.operation, "sin") == 0) {
                result = sin(req.num1);
            } else if (strcmp(req.operation, "cos") == 0) {
                result = cos(req.num1);
            } else if (strcmp(req.operation, "tan") == 0) {
                result = tan(req.num1);
            } else if (strcmp(req.operation, "sqrt") == 0) {
                result = sqrt(req.num1);
            } else if (strcmp(req.operation, "pow") == 0) {
                result = pow(req.num1, req.num2);
            } else {
                // Unknown operation
                printf("Unknown operation: '%s'\n", req.operation);
                result = 0.0;
            }

            printf("Result sent: %.4f\n", result);

            // 6. Send back result
            sendto(sockfd, &result, sizeof(result), 0,
                   (struct sockaddr *)&clientAddr, addr_len);
        }
    }

    close(sockfd);
    return 0;
}
