#include "udp_server.h"
#include "../common/sys_define.h"
#include "../common/sys_network.h"
#include "child_process.h"

void udp_server_main()
{
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    dg_echo(sockfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

}
