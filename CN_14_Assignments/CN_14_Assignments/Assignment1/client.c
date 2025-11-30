#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
	int sockfd;
	char buffer[100];
	struct sockaddr_in server;

	// Tcp socket creation
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// server details (where to connect)
	server.sin_family = AF_INET;
	server.sin_port = htons(8080); // sever port
	server.sin_addr.s_addr = inet_addr("10.0.0.1"); // server ip (localhost)
							
	// Request connection to the server
	connect(sockfd, (struct sockaddr*)&server, sizeof(server));

	// Send message to server
	char msg[] = "Hi";
	write(sockfd, msg, strlen(msg) + 1);

	// Receive reply from server
	read(sockfd, buffer, sizeof(buffer));
	printf("Server says: %s\n", buffer);

	// close the socket
	close(sockfd);

	return 0;

}

