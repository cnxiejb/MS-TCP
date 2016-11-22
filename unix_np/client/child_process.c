#include "child_process.h"

void str_cli(FILE *fp,int sockfd)
{
    char sendline[MAXLINE],recvline[MAXLINE];
    int maxfd,n;
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
            if((n = recv(sockfd,recvline,MAXLINE,0)) ==0)
            {
                printf("server terminated the connect\n");
                exit(0);
            }
            else if(n < 0)
            {
                perror("recv");
                exit(0);
            }
            printf("recv from server %s\n",recvline);
            fputs(recvline,stdout);
        }

        if(FD_ISSET(fileno(fp),&rset))
        {
            if(fgets(sendline,MAXLINE,fp) ==NULL)
            {
                return;
            }
            printf("client get %s\n",sendline);
            if((n=send(sockfd,sendline,strlen(sendline),0)) < 0)
            {
                perror("send");
                exit(0);
            }else if(n == 0)
            {
                printf("server terminated the connect\n");
                exit(0);
            }
        }
    }
}
void dg_cli(FILE *fp,int sockfd,struct sockaddr *addr,socklen_t len)
{
    int n;
    char mesg[MAXLINE],recvline[MAXLINE +1];

    while(fgets(mesg,MAXLINE,fp) !=NULL)
    {
        sendto(sockfd,mesg,strlen(mesg),0,addr,len);

        n = recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);

        recvline[n] =0;
        fputs(recvline,stdout);
    }
}
