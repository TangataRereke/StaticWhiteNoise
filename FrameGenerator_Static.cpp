#ifndef FRAMEGENERATOR_STATIC_H
    #define FRAMEGENERATOR_STATIC_H
    #include "FrameGenerator_Static.h"
#endif

void FrameGenerator_Static::prepareInitialFrame(){
/*     gCurrentFormulaFrame = new FormulaFrame(0, 0, 0, 1, 1, 1, gWidth, gHeight, min(gWidth, gHeight), gWidth, gHeight, this);
    gCurrentFormulaFrame->setup(gFormula);
    addNewGraphicsObject(gCurrentFormulaFrame); */
    gCurrentImage = new StaticImage(0, 0, 1, 1, 1, 1, gWidth, gHeight, min(gWidth, gHeight), gWidth, gHeight, this);
    addNewGraphicsObject(gCurrentImage);
}


void FrameGenerator_Static::setupBackground(){
}

int FrameGenerator_Static::getSoundPoint(int pPosition, int pChannel, int pTotalSize){
    if(gIncreaseAmplification){
        gCurrentAmplification = gCurrentAmplification + gRate;
        if(gCurrentAmplification>=32000){
            gIncreaseAmplification  = !gIncreaseAmplification;
            gCurrentAmplification = 32000;
        }
    }else{
        gCurrentAmplification = gCurrentAmplification - gRate;
        if(gCurrentAmplification<=32000){
            gIncreaseAmplification = !gIncreaseAmplification;
            gCurrentAmplification = 10000;
        }
    }

    long long lValue = rand();
    if(pChannel%2==0){
        return sin(lValue) * gCurrentAmplification;
    }else{
        return cos(lValue) * gCurrentAmplification;
    }
}

void FrameGenerator_Static::prepareFrame(){
    gRate++;
    if(gCurrentImage!=0){
        gCurrentImage->forceRecompile();
    }


}


