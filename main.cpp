#include <iostream>
#ifndef FRAMEGENERATOR_STATIC_H
    #define FRAMEGENERATOR_STATIC_H
    #include "FrameGenerator_Static.h"
#endif
#ifndef FRAMESCREATOR_H
    #define FRAMSECREATOR_H
    #include "./IndependentFiles/FramesCreator.h"
#endif
#define Output_Path "/home/james/tmp/Static.mp4"


int main()
{
    short unsigned lSizeX = 1920;
    short unsigned lSizeY = 1080;
    short unsigned lFramesPerSecond = 30;
    if(true){
        lSizeX = 3840;
        lSizeY = 2160;
        lFramesPerSecond = 60;
    }else if(false){
        lSizeX = 640;
        lSizeY = 320;
        lFramesPerSecond = 15;

    }
    std::cout << lSizeX << "/" << lSizeY << " @ " << lFramesPerSecond << std::endl;
    short unsigned lMinutes = 10;
    
    long unsigned lFrames = lMinutes * lFramesPerSecond * 60;
    std::cout << "TOTAL MINUTES: " << lFrames / lFramesPerSecond / 60 << std::endl;
    FrameGenerator_Static lGenerator(lSizeX, lSizeY, min(lSizeX, lSizeY), lSizeX, lSizeY);
    FramesCreator lFramesCreator(Output_Path, lSizeX, lSizeY, 1, lFrames, lFramesPerSecond, &lGenerator);
    cout << Output_Path << endl;

}
