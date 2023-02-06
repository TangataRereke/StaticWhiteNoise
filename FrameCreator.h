#ifndef IFRAMEGENERATOR_H
        #define IFRAMEGENERATOR_H
        #include "IFrameGenerator.h"
#endif
extern "C"{
        #include "libavcodec/avcodec.h"
}
#include "libavcodec/avcodec.h"

class FrameCreator{
    public:
        FrameCreator(const short unsigned pWidth, const short unsigned pHeight, IFrameGenerator *pFrameGenerator);
        ~FrameCreator();
        void processFrame(const short unsigned pFrameNumber);
        AVFrame *gVideoFrame = 0;
        AVFrame *gAudioFrame = 0;     
        u_int16_t *gAudioSamples = 0;   
        void generateSound(AVFrame *pFrame, short unsigned pNumberOfChannels);
    private:
        std::string gPath = "";
        short unsigned gWidth = 0;
        short unsigned gHeight = 0;
        IFrameGenerator *gFrameGenerator;
        int gAudioBufferSize = 0;
        int gAudioFrameSize = 0;
};