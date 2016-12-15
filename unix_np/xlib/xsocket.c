#include "xsocket.h"
#include "../common/sys_network.h"

int Fcntl(int sockfd,int cmd,long arg)
{
    return fcntl(sockfd,cmd,arg);
}
int Select(int maxfd,fd_set *rset,fd_set *wret,fd_set *errset,struct timeval *timeout)
{
    return select(maxfd,rset,wret,errset,timeout);
}
int Shutdown(int sockfd,int how)
{
    return shutdown(sockfd,how);
}
char * sock_ntop(const struct sockaddr *sa,socklen_t salen)
{
    char portsrt[8];
    static char str[128];

    switch(sa->sa_family)
    {
    case AF_INET:
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)sa;

        if(inet_ntop(AF_INET,&sin->sin_addr,str,sizeof(str)) == NULL)
            return NULL;
        if(ntohs(sin->sin_port) != 0)
        {
            snprintf(portsrt,sizeof(portsrt),":%d",ntohs(sin->sin_port));
            strcat(str,portsrt);
        }
        return (str);

    }

    }
}
