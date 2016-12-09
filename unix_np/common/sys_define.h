#ifndef _H_SYS_DEFINE_
#define _H_SYS_DEFINE_

#define MAXLINE 4096
#define SERV_PORT 9877
#define LISTENQ 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <time.h>
#include <errno.h>

#define max( a, b ) ( ( a > b) ? a : b )
#define min( a, b ) ( ( a < b) ? a : b )

#ifndef INFTIM
#define INFTIM -1
#endif

typedef struct sockaddr SA;
#define UNIXSTR_PATH "/tmp/unix.str"

#endif
