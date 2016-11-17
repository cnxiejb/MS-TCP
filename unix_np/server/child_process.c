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
        send(sockfd,recv_buf,strlen(recv_buf),0);
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
void dg_echo(int sockfd,struct sockaddr *addr,socklen_t size)
{
   int n;
   socklen_t len;
   char mesg[MAXLINE];

   for(;;)
   {
       len = size;
       n = recvfrom(sockfd,mesg,MAXLINE,0,addr,&len);
       sendto(sockfd,mesg,strlen(mesg),0,addr,len);
   }
}
