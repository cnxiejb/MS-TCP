#include "sys_define.h"
#include "sys_network.h"
#include "sp_server.h"
#include <limits.h>

void poll_server_main()
{
    int i,maxi,listenfd,confd,sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr,servaddr;

    listenfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    listen(listenfd,LISTENQ);
    client[0].fd = listenfd;
    client[0].events=POLLRDNORM;
    for(i=1; i < OPEN_MAX; i++)
    {
        client[i].fd =-1;
    }
    maxi=0;

    for(;;)
    {
        nready = poll(client,maxi + 1,INFTIM);

        if(client[0].revents & (POLLRDNORM))
        {
            clilen = sizeof(cliaddr);
            if((confd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen)) < 0)
            {
                perror("accept");
                exit(1);
            }else
            {
                 char peer_ip[INET_ADDRSTRLEN];
                bzero(peer_ip,INET_ADDRSTRLEN);
                if(inet_ntop(AF_INET,&(cliaddr.sin_addr),peer_ip,INET_ADDRSTRLEN) <= 0)
                {
                    printf("inet_ntop erro \n");
                    exit(1);
                }
                printf("connect from address: %s\n",peer_ip);                
            }
            for(i=1; i < OPEN_MAX; i++)
            {
                if(client[i].fd < 0)
                {
                    client[i].fd = confd;
                    break;
                }
            }
            if(i == OPEN_MAX)
                printf("too many client\n");
            client[i].events= POLLRDNORM;
            if(i > maxi)
                maxi= i;
            if(--nready < 0)
                continue;
        }

        for(i=1;i <= maxi;i++)
        {
            if((sockfd = client[i].fd) < 0)
                continue;
            if(client[i].revents & (POLLRDNORM | POLLERR))
            {
                if((n=recv(sockfd,buf,MAXLINE,0)) < 0){
                    if(errno == ECONNRESET){
                        perror("recv");
                        close(sockfd);
                        client[i].fd = -1;
                    }else{
                        perror("recv");
                        exit(1);
                    }
                }else if(n == 0)
                {
                    printf("client terminated the connect\n");
                    close(sockfd);
                    client[i].fd = -1;
                }else
                {
                    send(sockfd,buf,MAXLINE,0);
                }
            }
            if(--nready < 0)
                continue;
        }
    }
}

void select_server_main()
{
    int listenfd,connfd;
    int maxi,maxfd,nready,client[FD_SETSIZE];
    int i,n;
    char recv_buf[MAXLINE];

    /*pid_t childpid;*/
    socklen_t chilen;
    struct sockaddr_in servaddr,childaddr;
    fd_set rset,allset;

    if((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    listen(listenfd,LISTENQ);
    /*signal(SIGCHLD,sig_child);//signal function*/
    maxfd = listenfd;
    maxi= -1;
    for(i=0; i< FD_SETSIZE; i++)
    {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd,&allset);

    for(;;)
    {
        rset = allset;
        nready = select(maxfd+1,&rset,NULL,NULL,NULL);

        if(FD_ISSET(listenfd,&rset))
        {
            chilen = sizeof(childaddr);
            if((connfd = accept(listenfd,(struct sockaddr *)&childaddr,&chilen)) < 0)
            {
                if(errno == EINTR)
                    continue;
                else
                {
                    perror("accept");
                    exit(-1);
                }
            }
            else
            {
                char peer_ip[INET_ADDRSTRLEN];
                bzero(peer_ip,INET_ADDRSTRLEN);
                if(inet_ntop(AF_INET,&(childaddr.sin_addr),peer_ip,INET_ADDRSTRLEN) <= 0)
                {
                    printf("inet_ntop erro \n");
                    exit(1);
                }
                printf("connect from address: %s\n",peer_ip);
            }
            for(i=0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            if(i == FD_SETSIZE)
            {
                printf("too many clients\n");
                exit(0);
            }

            FD_SET(connfd,&allset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;
            if(--nready < 0)
                continue;
        }
        for(i=0; i <= maxi; i++)
        {
            if(client[i] < 0)
                continue;
            if(FD_ISSET(client[i],&rset))
            {
                /*printf("socket can read %d \n",client[i]);*/
                if((n = recv(client[i],recv_buf,MAXLINE,0))==0)
                {
                    printf("client terminated connect\n");
                    close(client[i]);
                    FD_CLR(client[i],&allset);
                    client[i]=-1;
                }
                else if(n < 0)
                {
                    perror("recv");
                }
                else
                {
                    /*printf("server recv %s\n",recv_buf);*/
                    send(client[i],recv_buf,strlen(recv_buf),0);
                }
                if(--nready < 0)
                    break;
            }
        }
    }
    close(listenfd);
}
