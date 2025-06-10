#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

int	client[1024] = {-1};
int clientCount = 0;
char	buffer[1024] = {0};

void putStrFd(int fd, char *str)
{
	if (!str)
		return ;
	write(fd, str, strlen(str));
}

void printExit(char *str)
{
	if (!str)
		putStrFd(3, "arg error");
	else
		putStrFd(3, "error syscall");
	exit(1);
}

void sendToAll(const fd_set *fd_target, char *msg, int clientExcept)
{
	int i = 0;
	while (i <= clientCount)
	{
		if (client[i] != clientExcept && FD_ISSET(client[i], fd_target))
			send(client[i], msg, strlen(msg), 0);
		i++;
	}
}

int main(int ac, char *av[])
{
	if (ac != 2)
		printExit("arg error");

	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli;
	struct timeval  tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;


	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); 
	if (sockfd == -1)
		printExit(NULL);
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		printExit(NULL);

	if (listen(sockfd, 10) != 0)
		printExit(NULL);

	len = sizeof(cli);

	fd_set fd_all, fd_read, fd_write;
	FD_ZERO(&fd_all);
	FD_SET(sockfd, &fd_all);
	while (1)
	{
		connfd = 0;
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		while (connfd > 0)
		{
			sprintf(buffer, "server: %i joined the chat\n", clientCount);
			// tell all
			sendToAll(&fd_write, buffer, clientCount);

			// add
			client[clientCount] = connfd;
			clientCount++;
			FD_SET(connfd, &fd_all);
			bzero(buffer, 1024);
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		}

		fd_read = fd_all;
		fd_write = fd_all;
		select(clientCount + 1, &fd_read, &fd_write, NULL, &tv);

		// receive and send
		int i = 0;
		while (i <= clientCount)
		{
			if (FD_ISSET(client[i], &fd_read))
			{
				if (recv(client[i], buffer, 1024, 0) == 0)
				{
					FD_CLR(client[i], &fd_all);
					client[i] = -1;
				}
				else
				{
					sendToAll(&fd_write, buffer, i);
				}
				bzero(buffer, 1024);
			}
			i++;
		}
	}
}