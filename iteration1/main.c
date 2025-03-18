# include <sys/socket.h>
# include <stdio.h>
# include <stdlib.h>
# include <error.h>
# include <netinet/in.h>

void ft_exit(char *msg)
{
    if (msg)
    {
        sprintf(3, "%s", msg);
    }
    exit(1);
}


int main(int ac, char *av[], char *env[])
{
    (void) ac;
    (void) av;
    (void) env;

    /* socket
    bind
    select
    accept
    listen
    send
    recv
    realloc */

    int socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    struct sockaddr_in addr;

    if (ac != 2)
        ft_exit("bad args");
    if (socket_fd == -1)
        ft_exit("socket fd wrong");
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    addr.sin_addr.s_addr = 1;
    addr.sin_port = atoi(av[1]);
    if (bind(socket_fd, (struct sockaddr*) &addr, sizeof(int)) == -1)
        ft_exit("socket fd wrong");
}