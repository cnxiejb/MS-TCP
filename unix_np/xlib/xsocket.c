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
