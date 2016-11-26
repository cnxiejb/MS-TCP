#include "../common/sys_define.h"
#include "../common/sys_network.h"
#include "transport.h"
#include "child_process.h"

void tcp_client(char *ipaddr)
{
    struct sockaddr_in servaddr;
    int sockfd;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET,ipaddr,&servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(1);
    }
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    str_cli(stdin,sockfd);
    exit(0);
}
void udp_client(char *ipaddr)
{
     struct sockaddr_in servaddr;
    int sockfd;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET,ipaddr,&servaddr.sin_addr) <= 0)
    {
        perror("inet_pton");
        exit(1);
    }

    dg_cli(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));;
    exit(0);    
}
