#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif

#ifndef IFRAMEGENERATOR_H
    #define IFRAMEGENERATOR_H
    #include "IFrameGenerator.h"
#endif 
void IGraphicsObject::copyXYZFromMoveTo(){
    gX = gMoveToX;
    gY = gMoveToY;
    gZ = gMoveToZ;
}

IGraphicsObject::IGraphicsObject(double pX, double pY, double pZ, double pSizeX, double pSizeY, double pSizeZ, double pFactorX, double pFactorY, double pFactorZ, short unsigned pMaxX, short unsigned pMaxY, IFrameGenerator *pFrameGenerator){
    gX = pX;
    gY = pY;
    gZ = pZ;
    gMoveToX = pX;
    gMoveToY = pY;
    gMoveToZ = pZ;
    gSizeX = pSizeX;
    gSizeY = pSizeY;
    gSizeZ = pSizeZ;
    gOriginalX = pX;
    gOriginalY = pY;
    gOriginalZ = pZ;
    gOriginalSizeX = pSizeX;
    gOriginalSizeY = pSizeY;
    gOriginalSizeZ = pSizeZ;
    gFactorX = pFactorX;
    gFactorY = pFactorY;
    gFactorZ = pFactorZ;
    gMaxX = pMaxX;
    gMaxY = pMaxY;
    gFrameGenerator = pFrameGenerator;
}

void IGraphicsObject::displayOids(){
    std::cout << "             - " << this << " x: " << gX << ", y: " << gY << ", z: " << gZ << std::endl;
}


void IGraphicsObject::addObject(IGraphicsObject *pNew){
    pNew->gPreviousObject = this;
    pNew->gNextObject = gNextObject;
    if(gNextObject!=0){
        gNextObject->gPreviousObject = pNew;
    }
    gNextObject = pNew;
}

void IGraphicsObject::insertObjectBefore(IGraphicsObject *pNew){
    pNew->gPreviousObject = gPreviousObject;
    pNew->gNextObject = this;
    if(gPreviousObject!=0){
        gPreviousObject->gNextObject = pNew;
    }
    gPreviousObject = pNew;
    
}

bool IGraphicsObject::isInFrame(){
    if(gFrontPicture!=0){
        return gFrontPicture->getLeft()>=0&&gActualLeft<gFactorX&&gFrontPicture->getTop()>=0&&gActualTop<=gFactorY;
    }
    return gActualLeft>=0&&gActualLeft<gFactorX&&gActualTop>=0&&gActualTop<=gFactorY;
}


bool IGraphicsObject::moveXY(double pX, double pY){
    gMoveToX = pX;
    gMoveToY = pY;
    return movePicture();
}

bool IGraphicsObject::moveXYZ(double pX, double pY, double pZ){
    gMoveToX = pX;
    gMoveToY = pY;
    gMoveToZ = pZ;
    return movePicture();
}

bool IGraphicsObject::incrementX(double pX){
    gMoveToX = gMoveToX + pX;
    return movePicture();
}

bool IGraphicsObject::incrementY(double pY){
    gMoveToY = gMoveToY + pY;
    return movePicture();
}

void IGraphicsObject::checkForReposition(){
    if(gMoveToX!=gX||gMoveToY!=gY||gMoveToZ!=gZ){
        gFrameGenerator->addObjectToMove(this);
    }
}

bool IGraphicsObject::movePicture(){
    double lStart = (1 - gMoveToZ) / 2;
    double lNewX = lStart + (gMoveToX * gMoveToZ);
    double lNewY = lStart + (gMoveToY * gMoveToZ);
    gActualLeft = lNewX * gFactorX;
    gActualTop = lNewY * gFactorY;
    if(gFrontPicture!=0){
        gFrontPicture->moveXY(gActualLeft, gActualTop);
    }
    checkForReposition();
    return lNewX > 0 && lNewX < 1 && lNewY >= 0 && lNewY < 1;
}

bool IGraphicsObject::incrementZ(double pZ){
    gMoveToZ = gMoveToZ + pZ;
    return movePicture();
}

bool IGraphicsObject::incrementXYZ(double pX, double pY, double pZ){
    gMoveToX = gMoveToX + pX;
    gMoveToY = gMoveToY + pY;
    gMoveToZ = gMoveToZ + pZ;
    return movePicture();
}

void IGraphicsObject::deleteObject(){
    gDeleted = true;
    removeObject();
    delete this;
}

void IGraphicsObject::removeObject(){
    if(gPreviousObject!=0){
        gPreviousObject->gNextObject = gNextObject;
    }
    if(gNextObject!=0){
        gNextObject->gPreviousObject = gPreviousObject;
    }
    gPreviousObject = 0;
    gNextObject = 0;

}

void IGraphicsObject::deleteCachedPicture(PictureCache *pPictureCache){
    if(pPictureCache!=0){
        delete pPictureCache;
        pPictureCache = 0;
    }
}

void IGraphicsObject::deleteCachedPictures(){
    deleteCachedPicture(gFrontPicture);
}

void IGraphicsObject::createCachedPictures(short unsigned pWidth, short unsigned pHeight, short unsigned pLength){
    gFrontPicture = new PictureCache(gMaxX, gMaxY);
}

IGraphicsObject::~IGraphicsObject(){
    
    deleteCachedPictures();
}

void IGraphicsObject::recompile(){
    short unsigned lWidth = max((int)(gFactorX * gSizeX), 1);
    short unsigned lHeight = max((int)(gFactorY * gSizeY), 1);
    short unsigned lLength = gFactorZ * gSizeZ;
    deleteCachedPictures();
    createCachedPictures(lWidth, lHeight, lLength);
    compile(lWidth, lHeight, lLength);
   
    movePicture();
    gNeedRecompile = false;
}

void IGraphicsObject::drawObject(PictureCache *pData){
    if(gNeedRecompile){
        recompile();
    }
    gFrontPicture->copyTo(pData);
}


void IGraphicsObject::forceRecompile(){
    gNeedRecompile = true;
}
