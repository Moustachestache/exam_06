#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

static int	g_clients[1024] = {-1};
int g_maxclients = 0;

//	buffers
static char messagebuffer[50000] = {'\0'};
int	messagebuffercount = 0;
static char connectionbuffer[128] = {'\0'};

void putStr(int fd, char *str)
{
	write(fd, str, strlen(str));
}

void errQuit(char *str)
{
	if (str == NULL)
		putStr(STDERR_FILENO, "Fatal error\n");
	else
		putStr(STDERR_FILENO, str);
	exit(1);
}

void newConnections(fd_set *write, int id)
{
	int i = 0;
	
	while (g_clients[i] >= 0 && i <= 1024 && g_clients[i] != -1)
	{
		if (id != i)
		{
			if (FD_ISSET(g_clients[i], write))
			{
				bzero(connectionbuffer, 128);
				sprintf(connectionbuffer, "server: client %d just arrived\n", id);
				send(g_clients[i], connectionbuffer, strlen(connectionbuffer), 0);
			}
		}
		i++;
	}
}

int main(int ac, char *av[])
{
	if (ac != 2)
		errQuit("Wrong number of arguments\n");
	int sockfd, connfd;
	socklen_t len = 0;
	struct sockaddr_in servaddr, cli;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 5;

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (sockfd == -1)
		errQuit(NULL);
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		errQuit(NULL);

	if (listen(sockfd, 10) != 0)
		errQuit(NULL);

	fd_set fd_all, fd_read, fd_write;
	FD_ZERO(&fd_all);
	while (1)
	{
		bzero(messagebuffer, 50000);
		messagebuffercount = 0;
		//	accept
		len = sizeof(cli);
		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		while (connfd > 0)
		{
			g_clients[g_maxclients] = connfd;
			FD_SET(connfd, &fd_all);
			//	send to fd_all is not clean but fd_read isnt set properly at this time ...
			newConnections(&fd_all, g_maxclients);

			//	loop conditions
			g_maxclients++;
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		}
		fd_read = fd_all;
		fd_write = fd_all;
		//	select here
		select(g_clients[g_maxclients -1] + 1, &fd_read, &fd_write, NULL, &tv);

		// read and add to buff
		int i = 0;
		while (i <= g_maxclients)
		{
			if (FD_ISSET(g_clients[i], &fd_read))
			{
				// client has left (cleanup and send msg)
				char localbuffer[10000] = {'\0'};
				int received = recv(g_clients[i], localbuffer, 10000, 0);
				if (received == 0)
				{
					messagebuffercount += sprintf(messagebuffer + messagebuffercount, "server: client %d just left\n", i);
					FD_CLR(g_clients[i], &fd_all);
					FD_CLR(g_clients[i], &fd_write);
				}
				else
				{
					messagebuffercount += sprintf(messagebuffer + messagebuffercount, "client %d: %s", i, localbuffer);
				}
			}
			i++;
		}

		// send big buffer
		while (i >= 0)
		{
			if (FD_ISSET(g_clients[i], &fd_write))
			{
				send(g_clients[i], messagebuffer, messagebuffercount + 1, 0);
			}
			i--;
		}
	}
}