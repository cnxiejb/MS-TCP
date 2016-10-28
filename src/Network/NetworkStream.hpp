#ifndef _H_NETWORK_STREAM_
#define _H_NETWORK_STREAM_

class NetworkStream
{
    public:
        virtual ~NetworkStream();
        virtual void SendPacket()=0;
};
#endif
