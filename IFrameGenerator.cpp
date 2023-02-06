#ifndef IFRAMEGENERATOR_H
        #define IFRAMEGENERATOR_H
        #include "IFrameGenerator.h"
#endif

IGraphicsObject *IFrameGenerator::getFirstGraphicsObject(){
    return gAccessManager.getFirstGraphicsObject();
}

IGraphicsObject *IFrameGenerator::getNextGraphicsObject(IGraphicsObject *pGraphicsObject){
    return gAccessManager.getNextGraphicsObject(pGraphicsObject);
}

void IFrameGenerator::generateFrame(short unsigned pFrame){
    gCurrentFrame = pFrame;
    prepareFrame();
    bool lUpdated = gAccessManager.moveObjects();
    lUpdated = gAccessManager.deleteObjects() or lUpdated;
    if(lUpdated){
        gAccessManager.updateCounts();
    }
}

void IFrameGenerator::generateInitialFrame(const short unsigned pFrame){
    time_t t;
    srand((unsigned) time(&t));
    gCurrentFrame = pFrame;    
    prepareInitialFrame();
    setupBackground();
    if(gBackground){
        gBackground->compile();
    }

}
void IFrameGenerator::drawFrame(PictureCache *pData){
    if(gBackground){
        gBackground->copyTo(pData);
    }
    gAccessManager.drawFrame(pData);

}

void IFrameGenerator::deleteObject(IGraphicsObject *pDeleteObject){
    gAccessManager.addDeleteObject(pDeleteObject);
}

void IFrameGenerator::displayOids(const char *pDescription){
    gAccessManager.displayOids(pDescription);
}


IFrameGenerator::IFrameGenerator(double pFactorX, double pFactorY, double pFactorZ, short unsigned pWidth, short unsigned pHeight){
    gFactorX = pFactorX;
    gFactorY = pFactorY;
    gFactorZ = pFactorZ;
    gWidth = pWidth;
    gHeight = pHeight;
}

void IFrameGenerator::addObjectToMove(IGraphicsObject *pObject){
    gAccessManager.addMoveObject(pObject);
}

void IFrameGenerator::addNewGraphicsObject(IGraphicsObject *pNewGraphicsObject){
    gAccessManager.addNewObject(pNewGraphicsObject);
    return;
}
IFrameGenerator::~IFrameGenerator(){
    if(gBackground){
        delete gBackground;
    }
}
long long unsigned IFrameGenerator::getObjectCount(){
    return gAccessManager.getObjectCount();
}
