#include "../xlib/xsocket.h"

int main(int argc,char **argv)
{
    struct ifi_info *ifi,*ifihead;
    struct sockaddr *sa;
    u_char *ptr;
    int i,family,doaliases;

    if(argc != 3)
        err_quit("usage:printinfo <inet4|inet6> <doaliases>");

    if(strcmp(argv[1],"inet4") == 0)
        family = AF_INET;
    else if(strcmp(argv[1],"inet6") ==0)
        family = AF_INET6;
    else
        err_quit("invalid <address-family>");

    doaliases = atoi(argv[2]);

    for(ifihead = ifi = Get_ifi_info(family,doaliases);ifi != NULL;ifi=ifi->ifi_next)
    {
        printf("%s: ",ifi->ifi_name);
        if(ifi->ifi_index != 0)
            printf("(%d) ",ifi->ifi_index);
        printf("<");
        if(ifi->ifi_falgs & IFF_UP) printf("UP ");
        if(ifi->ifi_falgs & IFF_BROADCAST) printf("BCAST");
        if(ifi->ifi_falgs & IFF_MULTICAST) printf("MCAST");
        if(ifi->ifi_falgs & IFF_LOOPBACK) printf("LOOP");
        if(ifi->ifi_falgs & IFF_POINTOPOINT) printf("P2P");
        printf(">");

        if((i = ifi->ifi_hlen) > 0){
            ptr = ifi->ifi_haddr;
            do{
                printf("%s%x",(i == ifi->ifi_hlen) ? " " : ":",*ptr++);
            }while(--i > 0 );
            printf("\n");
        }
        if(ifi->ifi_mtu !=0)
            printf("MTU : %d\n",ifi->ifi_mtu);
    }
    free_ifi_info(ifihead);
    exit(0);
}
