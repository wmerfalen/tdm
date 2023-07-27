// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "payload.hpp"

#define PORT	 25033
#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("198.199.114.15");

	int n;
	socklen_t len;

	while(1) {
		sendto(sockfd, ip_utils::get_message().c_str(), ip_utils::get_message().length(),
		    MSG_CONFIRM, (const struct sockaddr *) &servaddr,
		    sizeof(servaddr));
		printf("message sent.\n");

		//n = recvfrom(sockfd, (char *)buffer, MAXLINE,
		//        MSG_WAITALL, (struct sockaddr *) &servaddr,
		//        &len);
		//buffer[n] = '\0';
		//printf("Server : %s\n", buffer);
		usleep(850000);
	}

	close(sockfd);
	return 0;
}

