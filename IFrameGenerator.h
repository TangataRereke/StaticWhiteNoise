#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif
#ifndef IBACKGROUND_H
    #define IBACKGROUND_H
    #include "IBackground.h"
#endif
#ifndef AXISMANAGER_H
    #define AXISMANAGER_H
    #include "AxisManager.h"
#endif

#include <iostream>
#include <list>
class IFrameGenerator{
    public:
        void generateFrame(short unsigned pFrameNumber);
        void generateInitialFrame(short unsigned pFrameNumber);
        IFrameGenerator(double pFactorX, double pFactorY, double pFactorZ, short unsigned pWidth, short unsigned pHeight);
        void addNewGraphicsObject(IGraphicsObject *pNewObject);
        void deleteObject(IGraphicsObject *pObject);
        void drawFrame(PictureCache *pData);
        void displayOids(const char *pName);
        virtual void prepareFrame() = 0;
        IGraphicsObject *getFirstGraphicsObject();
        IGraphicsObject *getNextGraphicsObject(IGraphicsObject *pGraphicsObject);
        AxisManager gAccessManager;
        void addObjectToMove(IGraphicsObject *pObjectToMove);
        long long unsigned getObjectCount();
        virtual int getSoundPoint(int pPosition, int pChannel, int pTotalSize) = 0;
    protected:
        virtual void setupBackground() = 0;
        IBackground *gBackground = 0;
        short unsigned gCurrentFrame = 0;
        virtual void prepareInitialFrame() = 0;
        short unsigned gWidth = 0;
        short unsigned gHeight = 0;
        ~IFrameGenerator();
    private:
        double gFactorX = 0;
        double gFactorY = 0;
        double gFactorZ = 0;
};