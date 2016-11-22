#ifndef _H_SYS_DEFINE_
#define _H_SYS_DEFINE_

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
#include<errno.h>

#define max( a, b ) ( ( a > b) ? a : b )

#ifndef INFTIM
#define INFTIM -1
#endif

#endif
