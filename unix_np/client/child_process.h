#ifndef _H_CHILD_PRO_
#define _H_CHILD_PRO_

#include <stdio.h>
#include "sys_network.h"

void str_cli(FILE *fp,int sockfd);
void dg_cli(FILE *fp,int sockfd,struct sockaddr *addr,socklen_t size);
#endif
