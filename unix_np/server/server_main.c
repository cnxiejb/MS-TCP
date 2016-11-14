#include "sys_network.h"
#include "sys_define.h"
#include "child_process.h"
#include "sys_signal.h"

int main(int argc,char **argv)
{
    int listenfd,connfd,n;
    pid_t childpid;
    socklen_t chilen;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr,childaddr;

    /*printf("socket\n");*/
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
    signal(SIGCHLD,sig_child);//signal function

    for(;;){
        chilen = sizeof(childaddr);
        connfd = accept(listenfd,(struct sockaddr *)&childaddr,&chilen);
        if(errno == EINTR)
            continue;
        else
        {
            perror("accept");
            exit(-1);
        }
        if((childpid=fork()) == 0){//child process
            close(listenfd);
            str_echo(connfd);
            close(connfd);
            exit(0);
        }else{
            char peer_ip[INET_ADDRSTRLEN];
            bzero(peer_ip,INET_ADDRSTRLEN);
            if(inet_ntop(AF_INET,&(childaddr.sin_addr),peer_ip,INET_ADDRSTRLEN) <= 0){
                printf("inet_ntop erro \n");
                return -2;
            }
            printf("connect from address: %s\n",peer_ip);
            close(connfd);
        }
        
    }
    close(listenfd);
    return 0;
    
}
