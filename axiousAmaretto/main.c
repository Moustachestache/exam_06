# include <sys/socket.h>
# include <stdio.h>
# include <strings.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <sys/select.h>

/* goal:
    - receive and print a message sent from nc
*/

int main (void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    int maxfd = sockfd;
	int connfd, len;
	struct sockaddr_in servaddr, cli;

	bzero(&servaddr, sizeof(servaddr)); 

	if (sockfd == -1) 
    {
        printf("sockfdError\n");
        exit(1);
    }

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(8081); 

	if (listen(sockfd, 10) != 0)
    {
		printf("listenError\n"); 
		exit(1); 
	}

    fd_set fd_set_backup, fd_set_read, fd_set_write;
    FD_ZERO(&fd_set_backup);
    char readbuff[1024];
    bzero(readbuff, 1024);
    struct timeval caca;
    caca.tv_sec = 0;
    caca.tv_usec = 0;
    while (1)
    {
        //  accept sockets
        socklen_t len = sizeof(cli);
        connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
        while (connfd > 0)
        {
            FD_SET(connfd, &fd_set_backup);
            connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
            maxfd = connfd;
            printf("new connection on fd %i", connfd);
        }
        fd_set_read = fd_set_backup;

        /* int select(int nfds, fd_set *_Nullable restrict readfds,
                  fd_set *_Nullable restrict writefds,
                  fd_set *_Nullable restrict exceptfds,
                  struct timeval *_Nullable restrict timeout); */
        select(maxfd + 1, &fd_set_read, &fd_set_write, NULL, &caca);
        //  read
        int i = 0;
        while(i < maxfd + 1)
        {
            if (FD_ISSET(i, &fd_set_read) != 0)
            {
                recv(i, readbuff, 1024, 0);
                printf("%i says: %s\n", i, readbuff);
            }
            i++;
        }

        //  write
    }
}