#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

int client[1024] = {-1};
int clientCount = 0;

void errExit()
{
	printf("error\n");
	exit(1);
}

void	sendAll(fd_set *fd_write, char *buffer, int except)
{
	int i = 0;
	while (i <= clientCount)
	{
		if (FD_ISSET(client[i], fd_write) && i != except)
		{
			send(client[i], buffer, strlen(buffer), 0);
		}
		i++;
	}
}

int main(int ac, char *av[])
{
	if (ac != 2)
		errExit();

	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli; 

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); 
	if (sockfd == -1)
		errExit();
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		errExit();
	if (listen(sockfd, 10) != 0)
		errExit();

	len = sizeof(cli);
	char buffer[1024] = {0};

	fd_set fd_all, fd_read, fd_write;
	FD_ZERO(&fd_all);
	
	while (1)
	{
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		// handle connections
		if (connfd > 0)
		{
			client[clientCount] = connfd;
			FD_SET(connfd, &fd_all);
			sprintf(buffer, "client %i has joined\n", clientCount);
			sendAll(&fd_write, buffer, clientCount);
			clientCount++;
		}
		fd_read = fd_all;
		fd_write = fd_all;
		select(client[clientCount], &fd_read, &fd_write, NULL, &tv);
		// send messages as received
		int j = 0;
		while (j < clientCount)
		{
			bzero(buffer, 1024);
			if (recv(client[j], buffer, 1024, 0) == 0)
			{
				FD_CLR(client[j], &fd_all);
				client[j] = -1;
				sprintf(buffer, "server: %i left\n", j);
			}
			sendAll(&fd_write, buffer, j);
			j++;
		}
	}
}