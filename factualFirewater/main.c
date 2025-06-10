#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

errExit()
{
	printf("error!");
	exit(1);
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
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
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


	fd_set fd_all, fd_read, fd_write;
	FD_ZERO(&fd_all);

	int client[1024] = {-1};
	int clientCount = 0;
	
	len = sizeof(cli);
	while (1)
	{
		//	new connections and messages
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		if (connfd > 0)
		{

		}
	}
}

/* 
while (1)
{
	//	new accept

	// iterate all clients as read write
	while (j <= clientNumber)
	{
		j++;
	}
}
*/