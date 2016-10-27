#ifndef _V_INTERFACE_
#define _V_INTERFACE_

class videoInterface
{
    public:
        virtual int init()=0;
        virtual int Uninit()=0;

        void process(int type);
};
#endif
