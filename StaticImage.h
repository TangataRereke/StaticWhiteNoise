#ifndef PICTURECACHE_H
    #define PICTURECACHE_H
    #include "PictureCache.h"
#endif
#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif


class StaticImage : public IGraphicsObject {
    public:
        StaticImage(double pX, double pY, double pZ, double pSizeX, double pSizeY, double pSizeZ, double pFactorX, double pFactorY, double pFactorZ, short unsigned pMaxX, short unsigned pMaxY, IFrameGenerator *pFrameGenerator) : IGraphicsObject(pX, pY, pZ, pSizeX, pSizeY, pSizeZ, pFactorX, pFactorY, pFactorZ, pMaxX, pMaxY, pFrameGenerator){};
        void compile(short unsigned pWidth, short unsigned pHeight, short unsigned pLength);
      
    private:
        
};