#include<sys/time.h>
#include "util.h"
#include "../common/sys_network.h"
#include "../common/sys_define.h"

char *gf_time(void)
{
    struct timeval tv;
    static char str[30];
    char *ptr;

    if(gettimeofday(&tv,NULL) < 0)
    {
        perror("gettimeofday");
        exit(-1);
    }
    ptr = ctime(&tv.tv_sec);
    strcpy(str,&ptr[11]);

    snprintf(str + 8,sizeof(str)-8,".%06d",tv.tv_usec);
    return(str);
}
