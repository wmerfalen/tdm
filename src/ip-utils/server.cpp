// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "payload.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <set>

#define PORT	 25033
#define MAXLINE 1024

std::vector<std::string> ip_addresses;

// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if(bind(sockfd, (const struct sockaddr *)&servaddr,
	        sizeof(servaddr)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int n;

	socklen_t len = sizeof(cliaddr); //len is value/result

	while(1) {
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
		        MSG_WAITALL, (struct sockaddr *) &cliaddr,
		        &len);
		buffer[n] = '\0';
		std::string f = (char*)&buffer[0];
		if(f.compare(ip_utils::get_message().c_str()) == 0) {
			std::cout << "got message: " << inet_ntoa(cliaddr.sin_addr) << "\n";
			ip_addresses.emplace_back(inet_ntoa(cliaddr.sin_addr));
			std::set<std::string> ips;
			for(const auto& ip : ip_addresses) {
				ips.insert(ip);
			}
			if(ips.size() > 1) {
				system("./ip-changed.sh");
				ip_addresses.clear();
				continue;
			} else {
				ip_addresses.clear();
				ip_addresses.emplace_back(*ips.begin());
			}

		}
		sendto(sockfd, ip_utils::get_message().c_str(), ip_utils::get_message().length(),
		    MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
		    len);
		usleep(150000);
	}

	return 0;
}

