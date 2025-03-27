#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>

/* receive messages
	server sends received messages to all */

int main() {
	int sockfd, connfd;
	socklen_t len;
	int maxFd;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	maxFd = sockfd;
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(8081); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n");
	if (listen(sockfd, 10) != 0) {
		printf("cannot listen\n"); 
		exit(0); 
	}
	if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n");

	fd_set fd_all, fd_write, fd_read;
	FD_ZERO(&fd_all);
	FD_SET(connfd, &fd_all);
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_read);

	char buffer[1024];
	while (1)
	{
		printf("loop!\n");
		fd_write = fd_all;
		fd_read = fd_all;
		bzero(buffer, 1024);

		//	accept new con
		len = sizeof(cli);
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		while (connfd > 0)
		{
			FD_SET(connfd, &fd_all);
			//	sendAllNewConnection
			printf("new connection on fd %i\n", connfd);
			maxFd = connfd;
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		}
		select(maxFd + 1, &fd_read, &fd_write, NULL, (void *)0);
		//	read
		int i = 0;
		while (i < maxFd + 1)
		{
				printf("pipi!\n");
			if (FD_ISSET(i, &fd_write) > 0)
			{
				printf("isset!\n");
				if (recv(i, buffer, 1024, 0) == 0)
				{
					printf("%i has left the chatte\n", connfd);
					FD_CLR(i, &fd_all);
				}
				else
				{
					printf("%i: %s\n", connfd, buffer);
				}
				bzero(buffer, 1024);
			}
			i++;
		}

		//	write
	}
}