#ifndef _H_XSOCKET_
#define _H_XSOCKET_

#include "../common/sys_network.h"
#include "../common/sys_define.h"
#include "util.h"
#include "error.h"

#include "unpifi.h"

int connect_timeo(int sockfd,const SA *saptr,socklen_t salen,int nsec);
int connect_nonb(int sockfd,const SA *saptr,socklen_t salen, int nsec);

int Fcntl(int sockfd,int cmd,long arg);
int Select(int sockfd,fd_set *,fd_set *,fd_set *,struct timeval *);
int Shutdown(int sockfd,int how);

//select read timeo function
int readable_timeo(int fd,int sec);

//for webTest connect noblock
int web_main(int argc,char **argv);

int Tcp_connect(const char *host,const char *serv);
struct addrinfo * Host_serv(const char *host,const char *serv,int family,int socktype);
#endif
