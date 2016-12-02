#ifndef _H_CHILD_PRO_
#define _H_CHILD_PRO_

#include "../common/sys_define.h"
#include "../common/sys_network.h"

void str_cli(FILE *fp,int sockfd);
void str_cli_select_block(FILE *fp,int sockfd);
void str_cli_select_noblock(FILE *fp,int sockfd);
void str_cli_fork(FILE *fp,int sockfd);
void str_cli_thread();

void dg_cli(FILE *fp,int sockfd,struct sockaddr *addr,socklen_t size);
#endif
