#include "sys_network.h"
#include "sys_define.h"
#include "child_process.h"
#include "sys_signal.h"
#include "sp_server.h"
#include "udp_server.h"

int main(int argc,char **argv)
{
    /*select_server_main();*/
    /*poll_server_main();*/
    udp_server_main();
}
