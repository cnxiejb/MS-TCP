#ifndef _H_C_PROCESS_
#define _H_C_PROCESS_
#include "sys_network.h"

void str_echo(int sockfd);
void dg_echo(int sockfd,struct sockaddr *addr,socklen_t size);

#endif
