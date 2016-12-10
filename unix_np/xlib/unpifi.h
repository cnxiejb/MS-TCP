#ifndef UNP_IF_H_
#define UNP_IF_H_

#include "../common/sys_define.h"
#include "../common/sys_network.h"
#include <net/if.h>

#define IFI_NAME 16
#define IFI_HADDR 8

struct ifi_info{
    char ifi_name[IFI_NAME];
    short ifi_index;
    short ifi_mtu;
    u_char ifi_haddr[IFI_HADDR];
    u_short ifi_hlen;
    short ifi_falgs;
    short ifi_myflags;
    struct sockaddr *ifi_addr;
    struct sockaddr *ifi_brdaddr;
    struct sockaddr *ifi_dstaddr;
    struct ifi_info *ifi_next;
};

#define IFI_ALIAS 1

struct ifi_info *Get_ifi_info(int,int);
void free_ifi_info(struct ifi_info *);

#endif
