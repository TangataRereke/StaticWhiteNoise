#include <string>
#include <iostream>
#include <future>

extern "C"{
        #include "libavcodec/avcodec.h"
        #include "libavformat/avformat.h"
        #include "libavcodec/avcodec.h"
        
        #include "libavutil/channel_layout.h"
        #include "libavutil/common.h"
        #include "libavutil/frame.h"
        #include "libavutil/opt.h"
        #include "libavutil/samplefmt.h"        
        #include <libswscale/swscale.h>
        #include <libswresample/swresample.h>
}

#ifndef IFRAMEGENERATOR_H
        #define IFRAMEGENERATOR_H
        #include "../IFrameGenerator.h"
#endif
#ifndef FRAMECREATOR_H
        #define FRAMECREATOR_H
        #include "../FrameCreator.h"
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"

#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/frame.h"
#include "libavutil/samplefmt.h"      

class FramesCreator{
    public:
        FramesCreator(std::string pPath, short unsigned pResolutionX, short unsigned pResolutionY, long unsigned pStartFrame, long unsigned pEndFrame, int pFramesPerSecond, IFrameGenerator *pFrameGenerator);
        ~FramesCreator();
    private:
        int gCurrentDTS = 1;
        int gCurrentPTS = 2;
        AVCodec *gVideoCodec = 0;
        AVCodec *gAudioCodec = 0;
        const short unsigned THREADS = 1;
        void showTimeLeft(time_t pTimer);
        void generateFrames();
        long unsigned gStartFrame = 0;
        long unsigned gEndFrame = 0;
        long unsigned gCurrentFrame = 0;
        short unsigned gWidth = 0;
        short unsigned gHeight = 0;
        int gFramesPerSecond = 0;
        IFrameGenerator *gFrameGenerator = 0;
        FrameCreator *gFrameCreator = 0;
        void createFrameCreators();
        AVFrame *generateFrame(FrameCreator *pFrameCreator, short unsigned pFrame);

        AVFormatContext *gFormatContext= 0;

        // a wrapper around a single output AVStream
        typedef struct OutputStream {
                AVStream *st;
                AVCodecContext *enc;
        
                /* pts of the next frame that will be generated */
                int64_t next_pts;
                int samples_count;
                
                AVFrame *frame;
                AVFrame *tmp_frame;
                
                AVPacket *tmp_pkt;
                 
                struct SwsContext *sws_ctx;
                struct SwrContext *swr_ctx;
                float t, tincr, tincr2;
        } OutputStream;

        bool gEncodeVideo = false;
        bool gEncodeAudio = false;

        OutputStream gVideoOutputStream;        
        OutputStream gAudioOutputStream;
        void add_stream(OutputStream *pOutputStream, AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       enum AVCodecID pCodec_id);

        void open_video(AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       OutputStream *pOutputStream, AVDictionary *pOptions);

        AVFrame *alloc_picture(enum AVPixelFormat pPix_fmt, int pWidth, int pHeight);
        void open_audio(AVFormatContext *pFormatContext, const AVCodec *pCodec,
                       OutputStream *pOutputStream, AVDictionary *pOptions);

        int generateVideoFrame(AVFormatContext *pFormatContext, AVCodecContext *pCodecContext, AVStream *pAVStream, AVPacket *pPacket);

        AVFrame *alloc_audio_frame(enum AVSampleFormat pSample_fmt,
                                 
                                  int pSample_rate, int pNb_samples);     

        int generateAudioFrame(AVFormatContext *pOutputContext, OutputStream *pOutputStream);
        AVFrame *get_audio_frame(OutputStream *oOutputStream);
        bool gUp = true;
        int write_frame(AVFormatContext *pFormatContext, AVCodecContext *pCodecContext, AVStream *pAVStream, AVFrame *pFrame, AVPacket *pPacket);
};
