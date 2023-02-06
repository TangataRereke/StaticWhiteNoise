#ifndef IBACKGROUND_H
    #define IBACKGROUND_H
    #include "IBackground.h"
#endif
IBackground::IBackground(short unsigned pWidth, short unsigned pHeight){
    gWidth = pWidth;
    gHeight = pHeight;
    gFactorY = min(gWidth, gHeight);
    gPictureCache = new PictureCache(gWidth, gHeight);
}

void IBackground::compile(){
    prepare();
    drawFrame(gPictureCache);
}

IBackground::~IBackground(){
    if(gPictureCache!=0){
        delete gPictureCache;
    }
}
void IBackground::copyTo(BYTE *pData){
    gPictureCache->copyTo(pData);
}
void IBackground::copyTo(PictureCache *pData){
    gPictureCache->copyTo(pData);
}
double IBackground::calculateBrightness(short unsigned pX, short unsigned pY){
    double lX = (double)pX / gWidth;
    double lY = (double)pY / gHeight;
    lX = abs(.5 - lX);
    lY = abs(.5 - lY);
    double lZ = (lX * lX) * (lY * lY);
    return lZ*255;
    //double lZ = abs(0.5 - (lX * lY));
    /* double lZ = (abs(.5 - lX) * abs(.5 - lY));
    double lStart = (1 - lZ) / 2;
    return lStart * 255;
    double lNewX = 1 - (lStart + (lX * lZ));
    double lNewY = 1 - (lStart + (lY * lZ));
    return calculateBrightness(lNewX, lNewY, lZ);  */
} 
double IBackground::calculateBrightness(double pX, double pY, double pZ){
    return ((pX + pY + pZ)/3) * 255;
}
