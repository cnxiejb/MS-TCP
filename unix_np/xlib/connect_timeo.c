#include "xsocket.h"
#include "sys_signal.h"

static void connect_alarm(int);

int connect_timeo(int sockfd,const SA * saptr,socklen_t salen,int nsec)
{
    Sigfunc *sigfunc;
    int n;

    sigfunc = signal(SIGALRM,connect_alarm);
    if(alarm(nsec) !=0)
        perror("alarm");
    if((n=connect(sockfd,saptr,salen)) < 0){
        close(sockfd);
        if(errno == EINTR)
            errno = ETIMEDOUT;
    }
    alarm(0);
    signal(SIGALRM,sigfunc);
    return(n);
}
static void connect_alarm(int signo)
{
    return;
}
