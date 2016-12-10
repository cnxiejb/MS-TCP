#ifndef TRANSPORT_S_H
#define TRANSPORT_S_H

//for udp
void udp_server_main();

//for tcp
void select_server_main();
void poll_server_main();

//for unix
void unix_domin_server_main01(int argc,char **argv);
void unix_domin_server_main02();
#endif
