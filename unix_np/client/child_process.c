#include "child_process.h"
#include "../xlib/xsocket.h"
#include "../xlib/sys_signal.h"

void str_cli_select_block(FILE *fp,int sockfd)
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
            }
            else if(n == 0)
            {
                printf("server terminated the connect\n");
                exit(0);
            }
        }
    }
}
void str_cli_select_noblock(FILE *fp,int sockfd)
{
    int maxfdpl,val,stdineof;
    ssize_t n,nwriten;
    fd_set rset,wset;
    char to[MAXLINE],fr[MAXLINE];
    char *toiptr,*tooptr,*friptr,*froptr;

    val=Fcntl(sockfd,F_GETFL,0);
    Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val=Fcntl(STDIN_FILENO,F_GETFL,0);
    Fcntl(STDIN_FILENO,F_SETFL, val | O_NONBLOCK);

    val=Fcntl(STDOUT_FILENO,F_GETFL,0);
    Fcntl(STDOUT_FILENO,F_SETFL, val | O_NONBLOCK);

    toiptr=tooptr=to;
    friptr=froptr=fr;
    stdineof=0;

    maxfdpl = max(max(STDOUT_FILENO,STDIN_FILENO),sockfd)+1;
    for(;;)
    {
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if(stdineof == 0 && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO,&rset);
        if(friptr < &fr[MAXLINE])
            FD_SET(sockfd,&rset);
        if(tooptr != toiptr)
            FD_SET(sockfd,&wset);
        if(froptr != friptr)
            FD_SET(STDOUT_FILENO,&wset);

        Select(maxfdpl,&rset,&wset,NULL,NULL);

        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            if((n = read(STDIN_FILENO,toiptr,&to[MAXLINE]-toiptr))< 0)
            {
                if(errno != EWOULDBLOCK)
                    perror("read");
            }
            else if(n == 0)
            {
                fprintf(stderr,"%s:EOF on stdin\n",gf_time());
                stdineof=1;
                if(tooptr == toiptr)
                    Shutdown(sockfd,SHUT_WR);
            }
            else
            {
                fprintf(stderr,"%s:read %d bytes from stdin\n",gf_time(),n);
                toiptr += n;
                FD_SET(sockfd,&wset);
            }
        }

        if(FD_ISSET(sockfd,&rset))
        {
            if((n=read(sockfd,friptr,&fr[MAXLINE]-friptr)) < 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    perror("read socket");
                    exit(-1);
                }
            }
            else if(n ==0)
            {
                fprintf(stderr,"%s:EOF on socket\n",gf_time());
                if(stdineof)
                    return;
                else
                {
                    printf("server terminated the connect\n");
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr,"%s:read %d bytes from socket\n",gf_time(),n);
                friptr += n;
                FD_SET(STDOUT_FILENO,&wset);
            }
        }

        if(FD_ISSET(STDOUT_FILENO,&wset) && ((n= friptr-froptr) > 0))
        {
            if((nwriten = write(STDOUT_FILENO,froptr,n)) < 0)
            {
                if(errno != EWOULDBLOCK)
                {
                    perror("write stdout");
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr,"%s:wrote %d bytes to stdout\n",gf_time(),nwriten);
                froptr += nwriten;
                if(froptr == friptr)
                    froptr=friptr=fr;
            }
        }

        if(FD_ISSET(sockfd,&wset) && ((n = toiptr-tooptr) > 0))
        {
            if((nwriten = write(sockfd,tooptr,n)) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    perror("write socket");
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr,"%s:write %d bytes to socket\n",gf_time(),nwriten);
                tooptr += nwriten;
                if(tooptr == toiptr)
                {
                    tooptr=toiptr=to;
                    if(stdineof)
                        Shutdown(sockfd,SHUT_WR);

                }
            }
        }
    }
}
void str_cli_fork(FILE *fp,int sockfd)
{
    pid_t pid;
    char sendline[MAXLINE],recvline[MAXLINE];

    if((pid = fork()) == 0) //child process
    {
        while(read(sockfd,recvline,MAXLINE) > 0)
            fputs(recvline,stdout);
        kill(getppid(),SIGTERM);
        exit(0);
    }

    //parent process
    while(fgets(sendline,MAXLINE,fp) != NULL)
        write(sockfd,sendline,strlen(sendline));

    Shutdown(sockfd,SHUT_WR);
    pause();
    return;
}
void dg_cli(FILE *fp,int sockfd,struct sockaddr *addr,socklen_t len)
{
    int n;
    char mesg[MAXLINE],recvline[MAXLINE +1];
    if(connect(sockfd,addr,len) < 0)
    {
        perror("connect");
        exit(-1);
    }

    while(fgets(mesg,MAXLINE,fp) !=NULL)
    {
        /*sendto(sockfd,mesg,strlen(mesg),0,addr,len);*/
        send(sockfd,mesg,strlen(mesg),0);

        /*n = recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);*/
        n=recv(sockfd,recvline,MAXLINE,0);

        recvline[n] =0;
        fputs(recvline,stdout);
    }
}
static void sig_alrm(int signo)
{
    return;
}
void dg_cli_timeo(FILE *fp,int sockfd,const SA *addr,socklen_t len)
{
    int n;
    char mesg[MAXLINE],recvline[MAXLINE+1];

    Sigfunc *func=signal(SIGALRM,sig_alrm);
    while(fgets(mesg,MAXLINE,fp) != NULL)
    {
        sendto(sockfd,mesg,strlen(mesg),0,addr,len);

        alarm(5);
        if((n = recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL)) < 0)
        {
            if(errno ==EINTR)
                fprintf(stderr,"socket timeout\n");
            else
                err_sys("recvfrom error");
        }
        else
        {
            alarm(0);
            recvline[n]=0;
            fputs(recvline,stdout);
        }
    }
    signal(SIGALRM,func);
}
//use select for timeout
void dg_cli_readable_timeo(FILE *fp,int sockfd,const SA *addr,socklen_t len)
{
    int n;
    char mesg[MAXLINE],recvline[MAXLINE+1];

    while(fgets(mesg,MAXLINE,fp) != NULL)
    {
        sendto(sockfd,mesg,strlen(mesg),0,addr,len);

        if(readable_timeo(sockfd,5) == 0)
            fprintf(stderr,"sockfd timeout\n");
        else
        {
            n = recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
            recvline[n]=0;
            fputs(recvline,stdout);
        }
    }
}
void dg_cli_sockopt_timeo(FILE *fp,int sockfd,const SA *addr,socklen_t len)
{
    int n;
    char mesg[MAXLINE],recvline[MAXLINE];
    struct timeval tv;

    tv.tv_sec=5;
    tv.tv_usec=0;
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

    while(fgets(mesg,MAXLINE,fp) != NULL)
    {
        sendto(sockfd,mesg,strlen(mesg),0,addr,len);
        if((n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL)) < 0)
        {
            if(errno == EWOULDBLOCK)
            {
                fprintf(stderr,"socket recvfrom timeout\n");
                continue;
            }
            else
            {
                err_sys("recvfrom error");
            }
            recvline[0]=0;
            fputs(recvline,stdout);
        }
    }
}
