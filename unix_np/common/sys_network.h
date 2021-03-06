#ifndef _H_SYS_NET_
#define _H_SYS_NET_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//for select poll
#include <sys/select.h>
#include <poll.h>
//for unix domin socket
#include <sys/un.h>
//for fcntl
#include <fcntl.h>
//for getaddrinfo
#include <netdb.h>
//for ioctl
#include <sys/ioctl.h>
//for data link
#include <net/if_dl.h>
#include <net/if.h>
//for route
#include <net/route.h>

#endif
