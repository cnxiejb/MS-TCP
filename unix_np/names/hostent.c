#include "../common/sys_define.h"
#include "../common/sys_network.h"
#include<netdb.h>

int main(int argc,char **argv)
{
    char *ptr,**pptr;
    char str[INET_ADDRSTRLEN];
    struct hostent *hptr;

    while(--argc > 0){
        ptr = *++argv;
        if((hptr = gethostbyname(ptr))==NULL){
            perror("gethostbyname");
            continue;
        }
        printf("official name: %s\n",hptr->h_name);

        for(pptr=hptr->h_aliases;*pptr != NULL;pptr++){
            printf("\talias: %s\n",*pptr);
        }
        switch(hptr->h_addrtype){
            case AF_INET:
                pptr=hptr->h_addr_list;
                for(;*pptr != NULL;pptr++)
                    printf("\taddress:%s\n",inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
                break;
            default:
                printf("error type");
                break;
        }
    }
    exit(0);
}
