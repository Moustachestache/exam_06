#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

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
		putStrFd(3, "error syscall")
}


int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		printExit(NULL);
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(8081); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		printExit(NULL);
	if (listen(sockfd, 10) != 0)
		printExit(NULL);
	len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n");
}