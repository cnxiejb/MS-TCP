#include "child_process.h"
#include "sys_define.h"
#include "sys_network.h"

void str_cli(FILE *fp,int sockfd)
{
    char sendline[MAXLINE],recvline[MAXLINE];

    while(fgets(sendline,MAXLINE,fp) != NULL)
    {
        send(sockfd,sendline,strlen(sendline),0);
        printf("send %s\n",sendline);

        if(recv(sockfd,recvline,MAXLINE,0) == 0)
        {
            printf("str_cli:server terminated\n");
            exit(0);
        }
        printf("revc %s\n",recvline);
        fputs(recvline,stdout);
    }
}
