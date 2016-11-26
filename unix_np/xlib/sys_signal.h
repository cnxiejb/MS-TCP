#ifndef _H_SIGNAL_
#define _H_SIGNAL_

#include<signal.h>

typedef void Sigfunc(int);
Sigfunc* signal(int signo,Sigfunc *func);
void sig_child(int signo);

#endif
