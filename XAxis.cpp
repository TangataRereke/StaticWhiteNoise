#ifndef XAXIS_H
    #define XAXIS_H
    #include "XAxis.h"
#endif

void XAxis::displayOids(){
    std::cout << "         X Axis BEGIN " << gValue << std::endl;
        IGraphicsObject *lGraphicsObject = gAllGraphicsObjects;
        while(lGraphicsObject!=0){
            std::cout << "                             oid: " << lGraphicsObject << "; z: " << lGraphicsObject->getZ() << ", X: " << lGraphicsObject->getX() << ", Y: " << lGraphicsObject->getY() << std::endl;
            lGraphicsObject = lGraphicsObject->getNext();
        }
    std::cout << "         X Axis END " << gValue << std::endl;
    if(gNext!=0){
        gNext->displayOids();
    }
}
IGraphicsObject *XAxis::getNextGraphicsObject(IGraphicsObject *pGraphicsObject){
    if(pGraphicsObject->getNext()==0){
        if(gNext==0){
            return 0;
        }
    }
    return pGraphicsObject->getNext();
}

void XAxis::insertXObject(XAxis *pNewAxis){
    pNewAxis->gPrevious = gPrevious;
    pNewAxis->gNext = this;
    if(pNewAxis->gPrevious!=0){
        pNewAxis->gPrevious->gNext = pNewAxis;
    }
    gPrevious = pNewAxis;
}
void XAxis::addXObject(XAxis *pNewAxis){
    pNewAxis->gPrevious = this;
    gNext = pNewAxis;
}
void XAxis::addGraphicsObject(IGraphicsObject *pObject){
    gObjectCount++;
    if(gAllGraphicsObjects==0){
        gAllGraphicsObjects = pObject;
        gLastAddedGraphicsObject = pObject;
        return;
    }
    if(gLastAddedGraphicsObject!=0){
        gLastAddedGraphicsObject->addObject(pObject);
    }

}

void XAxis::removeGraphicsObject(IGraphicsObject *pObject){
    gObjectCount--;
    if(pObject==gAllGraphicsObjects){
        gAllGraphicsObjects = pObject->getNext();
    }
    if(pObject==gLastAddedGraphicsObject){
        gLastAddedGraphicsObject = pObject->getPrevious();
    }
}

XAxis::~XAxis(){
    IGraphicsObject *lGraphicsObject = gAllGraphicsObjects;
    while(lGraphicsObject!=0){
        IGraphicsObject *bNext = lGraphicsObject->getNext();
        delete lGraphicsObject;
        lGraphicsObject = bNext;
    }
}

void XAxis::deleteXObject(){
    if(gPrevious!=0){
        gPrevious->gNext = gNext;
    }
    if(gNext!=0){
        gNext->gPrevious = gPrevious;
    }
    delete this;
}