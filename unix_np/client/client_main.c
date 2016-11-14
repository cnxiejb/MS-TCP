/*************************************************************************
	> File Name: client_main.c
	> Author: 
	> Mail: 
	> Created Time: 四 11/10 15:13:22 2016
 ************************************************************************/
#include "child_process.h"
#include "sys_define.h"
#include "sys_network.h"

int main(int argc,char **argv)
{
    struct sockaddr_in servaddr;
    int sockfd;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        return -1;
    }

    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("connect");
        return -2;
    }
    
    str_cli(stdin,sockfd);
    exit(0);
}
