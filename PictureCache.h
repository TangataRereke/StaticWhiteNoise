#ifndef COLOURCLASS_H
    #define COLOURCLASS_H
    #include "IndependentFiles/ColourClass.h"
#endif
typedef unsigned char BYTE;
class PictureCache{
    public:
        PictureCache(short unsigned pWidth, short unsigned pHeight);
        void copyTo(BYTE *pData);
        void copyTo(PictureCache *pPictureCache);
        short getLeft(){return gLeft+gActualWidth;};
        short getTop(){return gTop+gActualHeight;};
        bool isInLimits();
        bool setPixel(short pX, short pY, ColourClass::ColourRef pColour);
        void moveXY(short pX, short pY){ gLeft = pX; gTop = pY;}
        ~PictureCache();
    protected:
        BYTE *gData = 0;
        unsigned int gSize = 0;
        short gActualLeft = 32000;
        short gActualTop = 32000;
        short gActualWidth = 0;
        short gActualHeight = 0;
        short gLeft = 0;
        short gTop = 0;
        short unsigned gWidth = 0;
        short unsigned gHeight = 0;
        unsigned int getStartBuffer(short unsigned pX, short unsigned pY);
};