#ifndef _H_DIFINE_
#define _H_DIFINE_

#define MAXLINE 4096
#define SERV_PORT 9877
#define LISTENQ 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/select.h>
#include <time.h>
#include <poll.h>

#define max( a, b ) ( ( a > b) ? a : b )

#ifndef INFTIM
#define INFTIM -1
#endif

void select_server_main();
void poll_server_main();
#endif
