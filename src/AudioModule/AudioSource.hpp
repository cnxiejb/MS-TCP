#ifndef _H_AUDIO_SOURCE_
#define _H_AUDIO_SOURCE_

class AudioSource
{
    public:
        virtual void  Init()=0;
        virtual void  UnInit()=0;
    private:
        void process();
};
#endif
