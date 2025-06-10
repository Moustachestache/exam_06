#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

// variables - fdsets, msgs, clients
FD_SET	fd_all, fd_read, fd_write;

typedef struct s_client {
	char	*msgs;
	int		fd;

} t_client;

t_client	*client[1000];

int		count = 0;
int		max_fd = 0;

char	buf_read[1001] = {0};
char	bug_msg[100] = {0};

// fatal_error void
void fatal_error()
{
	write(STDERR_FILENO, "Fatal error\n", 12);
	exit(1);
}

// create_socket void
int create_socket()
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1)
		fatal_error();
	count++;
	max_fd = sockfd;
}

// set_max_fd()
void	set_max_fd(int newfd)
{
	if (newfd > max_fd)
		max_fd = newfd;
}

// add_client void
t_client *new_client(int newfd)
{
	if (newfd <= 0)
		return NULL;

	t_client thisclient = malloc(sizeof(t_client));
	thisclient->fd = newfd;
	thisclient->msgs = NULL;

	client[count++] = thisclient;

	set_max_fd(newfd)

	//	message all
	sprintf(buf_msg, "client %d has joined\n", id);
	send_to_other(id, buf_msg);

	return (thisclient);
}

// remove_client *id
void remove_client(int id)
{
	sprintf(buf_msg, "client %d has left\n", id);
	send_to_other(id, buf_msg);

	if (client[id]->msg)
		free(client[id]->msg);
	if (client[id])
		free(client[id]);

	client[id] = NULL;
	//	message all
}

// send_all_msg *id

// send_to_other *id, *msg
void send_to_other(int id, char *msg)
{
	int i = 0;

	while (i < count)
	{
		if (i == id)
			continue ;

		if (client[i] && FD_SET(client[i]->fd, &fd_write))
			send(client[i]->fd, msg, strlen(msg), 0);
		i++;
	}
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}


int main(int ac, char *av[])
{
	if (ac != 2)
	{
		write(STDERR_FILENO, "Wrong number of arguments\n", 26);
		exit(1);
	}

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = create_socket();
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal_error();

	if (listen(sockfd, 10) != 0)
		fatal_error();

	len = sizeof(cli);

	int readsize = 0;
	while (1)
	{
		fd_write = fd_all;
		fd_read = fd_all;

		if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
			fatal_error();

		add_client(accept(sockfd, (struct sockaddr *)&cli, &len))

		int i = 0;
		while (i < count)
		{
			if (!FD_SET(client[i]->fd, &fd_read))
				continue ;

			// ssize_t recv(int sockfd, void buf[.len], size_t len, int flags);

			readsize = recv(client[i]->fd, );

			// reset
			i++;
			readsize = 0;
		}
	}
	return 0;
}