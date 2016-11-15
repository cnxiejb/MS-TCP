#include "child_process.h"
#include "sys_define.h"
#include "sys_network.h"

void str_cli(FILE *fp,int sockfd)
{
    char sendline[MAXLINE],recvline[MAXLINE];
    int maxfd;
    fd_set rset;

    for(;;)
    {
        FD_ZERO(&rset);
        FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfd = max(fileno(fp),sockfd)+1;
        select(maxfd,&rset,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&rset))
        {
            if(recv(sockfd,recvline,MAXLINE,0) ==0)
            {
                perror("recv");
                exit(0);
            }
            fputs(recvline,stdout);
        }

        if(FD_ISSET(fileno(fp),&rset))
        {
            if(fgets(sendline,MAXLINE,fp) ==NULL)
            {
                return;
            }
            send(sockfd,sendline,strlen(sendline),0);
        }
    }
}
