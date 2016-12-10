/*************************************************************************
	> File Name: client_main.c
	> Author:
	> Mail:
	> Created Time: 四 11/10 15:13:22 2016
 ************************************************************************/
#include "child_process.h"
#include "transport.h"

int main(int argc,char **argv)
{
/*    if(argc != 2)*/
    /*{*/
        /*printf("no server ip addr\n");*/
        /*exit(1);*/
    /*}*/

    /*udp_client(argv[1]);*/
    unix_domin_tcp_client();
    return 0;
}
