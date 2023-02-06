#ifndef PICTURECACHE_H
    #define PICTURECACHE_H
    #include "PictureCache.h"
#endif
#ifndef PICTURECACHEDRAW_H
    #define PICTURECACHEDRAW_H
    #include "PictureCacheDraw.h"
#endif

class IBackground{
    public:
        virtual void prepare() = 0;
        void compile();
        IBackground(short unsigned pWidth, short unsigned pHeight);
        ~IBackground();
        virtual void drawFrame(PictureCache *pPicture) = 0;
        short unsigned gWidth = 0;
        short unsigned gHeight = 0;
        void copyTo(BYTE *pData);
        void copyTo(PictureCache *pData);
        double calculateBrightness(short unsigned pX, short unsigned pY);
    private:
        double calculateBrightness(double pX, double pY, double pZ);
        PictureCache *gPictureCache = 0;
    protected:
        short unsigned gFactorY = 0;
};