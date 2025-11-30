#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sockfd, newsock;
	char buffer[100];
	struct sockaddr_in server, client;
	socklen_t client_len;

	// TCP socket creating
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// server address 
	server.sin_family = AF_INET; // IPv4
	server.sin_port = htons(8080); // server port
	server.sin_addr.s_addr = INADDR_ANY; //Accept from any interface
					     
	// Bind the socket to the given IP and port
	bind(sockfd, (struct sockaddr*)&server, sizeof(server));

	// Listen for incoming connections (queue size = 1)
	listen(sockfd, 1);

	// Accept a client's connection request
	client_len = sizeof(client);
	newsock = accept(sockfd, (struct sockaddr*)&client, &client_len);

	// Read message sent by client
	read(newsock, buffer, sizeof(buffer));
	printf("Client says: %s\n", buffer);

	// Send reply message to client
	char reply[] = "Hello";
	write(newsock, reply, strlen(reply) + 1);

	// close sockets
	close(newsock);
	close(sockfd);

	return 0;
}

