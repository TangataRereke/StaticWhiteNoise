#ifndef PICTURECACHE_H
    #define PICTURECACHE_H
    #include "PictureCache.h"
#endif

#ifndef PICTURECACHEDRAW_H
    #define PICTURECACHEDRAW_H
    #include "PictureCacheDraw.h"
#endif
#include <iomanip>

class IFrameGenerator;
class IGraphicsObject{
    public:
        IGraphicsObject(double pX, double pY, double pZ, double pSizeX, double pSizeY, double pSizeZ, double pFactorX, double pFactorY, double pFactorZ, short unsigned pMaxX, short unsigned pMaxY, IFrameGenerator *pGenerator);
        
        void displayOids();
        bool isInFrame();
        void insertObject(IGraphicsObject *pNew);
        void drawObjects(PictureCache *pData);
        void deleteObject();
        void drawColumn(PictureCache *pData);
        void drawObject(PictureCache *pData);
        bool moveXYZ(double pX, double pY, double pZ);
        bool incrementXYZ(double pX, double pY, double pZ);
        bool incrementX(double pX);
        bool incrementY(double pY);
        bool incrementZ(double pZ);
        virtual void compile(const short unsigned pWidth, const short unsigned pHeight, const short unsigned pLength) = 0;
        IGraphicsObject *getNext(){ return gNextObject;}
        double getX(){ return gX;};
        double getY(){ return gY;};
        double getZ(){ return gZ;};
        double getMoveToX(){ return gMoveToX;};
        double getMoveToY(){ return gMoveToY;};
        double getMoveToZ(){ return gMoveToZ;};
        bool moveXY(double pX, double pY);
        void forceRecompile();
        void addObject(IGraphicsObject *pNew);
        bool isDeleted(){return gDeleted;};
        IGraphicsObject *getPrevious(){ return gPreviousObject;};
        void copyXYZFromMoveTo();
       ~IGraphicsObject();
    protected:
        PictureCache *gFrontPicture = 0;

    private:
        IGraphicsObject *gNextObject = 0;
        IGraphicsObject *gPreviousObject = 0;   
        bool gDeleted = false; 
      void checkForReposition();
      void removeObject();
      void recompile();
      void insertObjectBefore(IGraphicsObject *pNew);
      void deleteCachedPictures();
      void createCachedPictures(short unsigned pWidth, short unsigned pHeight, short unsigned pLength);
      void deleteCachedPicture(PictureCache *pPictureCache);
      short gActualLeft = 0;
      short gActualTop = 0;
      bool movePicture();
      bool gNeedRecompile = true;
      short gMaxX = 0;
      short gMaxY = 0;
      double gRotationX = 0;
      double gRotationY = 0;
      double gX = 0;
      double gY = 0;
      double gZ = 0;
      double gMoveToX = 0;
      double gMoveToY = 0;
      double gMoveToZ = 0;
      double gSizeX = 0;
      double gSizeY = 0;
      double gSizeZ = 0;
      double gOriginalX = 0;
      double gOriginalY = 0;
      double gOriginalZ = 0;
      double gOriginalRotationX = 0;
      double gOriginalRotationY = 0;
      double gOriginalSizeX = 0;
      double gOriginalSizeY = 0;
      double gOriginalSizeZ = 0;
      double gFactorX = 0;
      double gFactorZ = 0;
      double gFactorY = 0;      
      IFrameGenerator *gFrameGenerator = 0;
};