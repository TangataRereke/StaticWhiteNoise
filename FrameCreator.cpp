#ifndef FRAMECREATOR_H
    #define FRAMECREATOR_H
    #include "FrameCreator.h"
#endif


FrameCreator::FrameCreator(const short unsigned pWidth, const short unsigned pHeight, IFrameGenerator *pFrameGenerator){
    gFrameGenerator = pFrameGenerator;
    gWidth = pWidth;
    gHeight = pHeight;
    
     
}

FrameCreator::~FrameCreator(){
    av_frame_free(&gVideoFrame);
   av_frame_free(&gAudioFrame); 

}

void FrameCreator::generateSound(AVFrame *pFrame, short unsigned pNumberOfChannels){
    int j, i, v;
    int16_t *q = (int16_t*)pFrame->data[0];
    for (j = 0; j <pFrame->nb_samples; j++) {
        for (i = 0; i < pNumberOfChannels; i++){
            v = gFrameGenerator->getSoundPoint(j, i, pFrame->nb_samples);
			*q++ = v;
		}
		
    }

}

void FrameCreator::processFrame(short unsigned pFrameNumber){
    gFrameGenerator->generateFrame(pFrameNumber);
    fflush(stdout);
    if(av_frame_make_writable(gVideoFrame)<0){
            std::cout << "Frame not writeable" << std::endl;
            exit(1);
    }
    PictureCache lPictureCache(gWidth, gHeight);
    gFrameGenerator->drawFrame(&lPictureCache);
    lPictureCache.copyTo(gVideoFrame->data[0]);
}
