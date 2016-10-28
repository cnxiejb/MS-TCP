#ifndef _H_RTMPPUBLISHER_
#define _H_RTMPPUBLISHER_

#include "NetworkStream.hpp"

class RtmpPublisher:public NetworkStream
{
    public:
        RtmpPublisher();
        ~RtmpPublisher();

        void SendPacket() override;
};
#endif
