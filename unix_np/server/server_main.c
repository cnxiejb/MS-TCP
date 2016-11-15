#include "sys_network.h"
#include "sys_define.h"
#include "child_process.h"
#include "sys_signal.h"

int main(int argc,char **argv)
{
    int listenfd,connfd,sockfd;
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
        return -1;
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
    {
        perror("bind");
        return -2;
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
            connfd = accept(listenfd,(struct sockaddr *)&childaddr,&chilen);
            if(errno == EINTR)
                continue;
            else
            {
                perror("accept");
                exit(-1);
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

            FD_SET(connfd,&rset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;
            if(--nready < 0)
                continue;
        }
        for(i=0;i < maxi;i++)
        {
            if(client[i] < 0)
                continue;
           if(FD_ISSET(client[i],&rset))
           {
               if((n = recv(client[i],recv_buf,MAXLINE,0))==0){
                   close(client[i]);
                   FD_CLR(client[i],&allset);
                   client[i]=-1;
               }else
                   send(client[i],recv_buf,strlen(recv_buf),0);
               if(--nready < 0)
                   break;
           }
        }
        /*if((childpid=fork()) == 0) //child process*/
        /*{*/
            /*close(listenfd);*/
            /*str_echo(connfd);*/
            /*close(connfd);*/
            /*exit(0);*/
        /*}*/
        /*else*/
        /*{*/
            /*char peer_ip[INET_ADDRSTRLEN];*/
            /*bzero(peer_ip,INET_ADDRSTRLEN);*/
            /*if(inet_ntop(AF_INET,&(childaddr.sin_addr),peer_ip,INET_ADDRSTRLEN) <= 0)*/
            /*{*/
                /*printf("inet_ntop erro \n");*/
                /*return -2;*/
            /*}*/
            /*printf("connect from address: %s\n",peer_ip);*/
            /*close(connfd);*/
        /*}*/

    }
    close(listenfd);
    return 0;

}
