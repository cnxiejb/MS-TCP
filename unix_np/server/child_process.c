#include <stdlib.h>
#include <stdio.h>
#include "child_process.h"
#include "sys_define.h"
#include "sys_network.h"

void str_echo(int sockfd)
{
    char recv_buf[MAXLINE];
    ssize_t n;

again:
    while((n = recv(sockfd,recv_buf,MAXLINE,0) > 0))
    {
        printf("server recv %s len %ldl\n",recv_buf,n);
        send(sockfd,recv_buf,n+1,0);
    }
    if(n<0 && errno == EINTR)
    {
        goto again;
    }
    else if(n < 0)
    {
        perror("recv");
        exit(-1);
    }

    if(n == 0)
    {
        printf("client terminated the connec\n");
        exit(0);
    }
}
