#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

//	globals
fd_set fd_all, fd_read, fd_write;
int	clients[1024] = {-1};
int	maxclients = 0;

void ft_error_exit(char *msg)
{
	if (msg == NULL)
		printf("Fatal error\n");
	else
		printf("Wrong number of arguments\n");
	exit(1);
}

//	sends msg when new connection
//	goes through all fd (except id which is the new conn.)
void	msgNewConn(int newId)
{
	int i = 0;
	char buffer1[16] = "server: client ";
	char buffer2[15] = " just arrived\n";
	char buffer3[4] = {'\0'};
	while (i < 1024 &&  clients[i] != -1)
	{
		if (clients[i] != 0)
		{
			send(clients[i], buffer1, strlen(buffer1), 0);
			send(clients[i], buffer2, strlen(buffer2), 0);
		}
		bzero(buffer3, 4);
	}
}

int main(int ac, char *av[], char *env[])
{
	if (ac != 2)
		ft_error_exit("oops");

	//	key is client id, content is the client fd;
	//	if fd is -1 then client is unavailable;

	int	maxFd = 0;
	
	int port = atoi(av[1]);
	int sockfd, connfd;
	socklen_t len;
	struct	sockaddr_in servaddr, cli;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sockfd == -1)
		ft_error_exit(NULL);
	bzero(&servaddr, sizeof(servaddr));
	maxFd = sockfd;

	// assign IP, PORT 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port);

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		ft_error_exit(NULL);

	if (listen(sockfd, 10) != 0)
		ft_error_exit(NULL);

	FD_ZERO(&fd_all);
	while (1)
	{
		// select and reset
		fd_read = fd_all;
		fd_write = fd_all;
		select(maxFd + 1, &fd_read, &fd_write, NULL, 0);

		//	incoming connections
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		while (connfd > 0)
		{
			FD_SET(connfd, &fd_all);
			clients[maxclients] = connfd;
			maxFd = connfd;
			msgNewConn(maxclients);

			//	loop conditions
			maxclients++;
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		}

		//	read + disconnect

		//	write

		//	debug sleep
		sleep(1);
	}
}