#ifndef AXISMANAGER_H
    #define AXISMANAGER_H
    #include "AxisManager.h"
#endif

void AxisManager::updateMoveObject(IGraphicsObject *pObject){
    ZAxis *lOldZAxis = getZAxisObject(pObject->getZ());
    if(lOldZAxis!=0){
        YAxis *lOldYAxis = lOldZAxis->getYAxisObject(pObject->getY());
        if(lOldYAxis!=0){
            XAxis *lOldXAxis = lOldYAxis->getXAxisObject(pObject->getX());
            if(lOldXAxis!=0){
                lOldXAxis->removeGraphicsObject(pObject);
            }
        }
    }
    obtainZAxisObject(pObject->getMoveToZ())->obtainYAxisObject(pObject->getMoveToY())->obtainXAxisObject(pObject->getMoveToX())->addGraphicsObject(pObject);
    pObject->copyXYZFromMoveTo();
}

bool AxisManager::deleteObjects(){
    bool lUpdated = false;
    for(IGraphicsObject *lGraphicsObject : gAllObjectsToDelete){
        lUpdated = true;
        ZAxis *lZAxis = getZAxisObject(lGraphicsObject->getZ());
        if(lZAxis){
            YAxis *lYAxis = lZAxis->getYAxisObject(lGraphicsObject->getY());
            if(lYAxis){
                XAxis *lXAxis = lYAxis->getXAxisObject(lGraphicsObject->getX());
                if(lXAxis!=0){
                    lXAxis->removeGraphicsObject(lGraphicsObject);
                    lGraphicsObject->deleteObject();
                }
            }
        }        
    }   
    gAllObjectsToDelete.clear();
    return lUpdated;
}

void AxisManager::updateCounts(){
    gObjectCount = 0;
    ZAxis *lAxis = gAllZAxisFrom0;
    list<ZAxis*> lDeleteObjects;
    while(lAxis){
        lAxis->updateCounts();
        if(lAxis->getObjectCount()==0){
            lDeleteObjects.push_back(lAxis);
        }else{
            gObjectCount = gObjectCount + lAxis->getObjectCount();
        }
        lAxis = lAxis->getNext();
    }
    deleteObjects(lDeleteObjects);
}

void AxisManager::deleteObject(ZAxis *pObject){
    if(pObject==gAllZAxisFrom0){
        gAllZAxisFrom0 = pObject->getNext();
    }
    if(pObject==gAllZAxisFromQuarter){
        gAllZAxisFromQuarter = pObject->getNext();
    }
    if(pObject==gAllZAxisFromHalf){
        gAllZAxisFromHalf = pObject->getNext();
    }
    if(pObject==gAllZAxisFromThreeQuarters){
        gAllZAxisFromThreeQuarters = pObject->getNext();
    }
    if(pObject==gAllZAxisFrom1){
        gAllZAxisFrom1 = pObject->getNext();
    }
    pObject->deleteObject();
}


void AxisManager::deleteObjects(list<ZAxis*> pObjects){
    for(ZAxis *lObject : pObjects){
        deleteObject(lObject);
    }
}

bool AxisManager::moveObjects(){
    bool lUpdated = false;
    for(IGraphicsObject *lGraphicsObject : gAllObjectsToMove){
        lUpdated = true;
        updateMoveObject(lGraphicsObject);
    }
    gAllObjectsToMove.clear();
    return lUpdated;
}

void AxisManager::addMoveObject(IGraphicsObject *pObject){
    gAllObjectsToMove.push_back(pObject);
}

void AxisManager::displayOids(const char *pName){
    std::cout << "Displaying OIDs for " << pName << std::endl;
    std::cout << " - 000 = " << gAllZAxisFrom0 << std::endl;
    std::cout << " - 1/4 = " << gAllZAxisFromQuarter << std::endl;
    std::cout << " - 1/2 = " << gAllZAxisFromHalf << std::endl;
    std::cout << " - 3/4 = " << gAllZAxisFromThreeQuarters<< std::endl;
    std::cout << " - 001 = " << gAllZAxisFrom1 << std::endl;    
        if(gAllZAxisFrom0!=0){
            gAllZAxisFrom0->displayOids();
        }
    std::cout << "Finished displaying OIDS " << pName << std::endl;
}

void AxisManager::addDeleteObject(IGraphicsObject *pObject){
    gAllObjectsToDelete.push_back(pObject);
}

ZAxis *AxisManager::obtainZAxisObject(double pValue){
    ZAxis *lAxis = getZAxisObject(pValue);
    if(lAxis){
        return lAxis;
    }
    return addZAxisObject(pValue);
}

void AxisManager::updateCollections(ZAxis *pNew){
    if(gAllZAxisFrom0!=0&&gAllZAxisFrom0->getValue()>pNew->getValue()){
        gAllZAxisFrom0 = pNew;
    }
    if(pNew->getValue()>=.25&&(gAllZAxisFromQuarter==0||gAllZAxisFromQuarter->getValue()>pNew->getValue())){
        gAllZAxisFromQuarter = pNew;
    }
    if(pNew->getValue()>=.5&&(gAllZAxisFromHalf==0||gAllZAxisFromHalf->getValue()>pNew->getValue())){
        gAllZAxisFromHalf = pNew;
    }
    if(pNew->getValue()>=.75&&(gAllZAxisFromThreeQuarters==0||gAllZAxisFromThreeQuarters->getValue()>pNew->getValue())){
        gAllZAxisFromThreeQuarters = pNew;
    }
    if(pNew->getValue()>=1&&(gAllZAxisFrom1==0||gAllZAxisFrom1->getValue()>pNew->getValue())){
        gAllZAxisFrom1 = pNew;
    }

}

ZAxis *AxisManager::addZAxisObject(double pValue){
    ZAxis *lNewAxis = new ZAxis(pValue);
    if(gAllZAxisFrom0==0){
        gAllZAxisFrom0 = lNewAxis;
        if(pValue>=.25){
            gAllZAxisFromQuarter = lNewAxis;
            if(pValue>=.5){
                gAllZAxisFromHalf = lNewAxis;
                if(pValue>=.75){
                    gAllZAxisFromThreeQuarters = lNewAxis;
                    if(pValue>=1){
                        gAllZAxisFrom1 = lNewAxis;
                    }
                }
                
            }
        }
        return lNewAxis;
    }
    ZAxis *lFrom = 0;
    if(pValue>=1){
        lFrom = gAllZAxisFrom1;
    }
    if(lFrom==0&&pValue>=.75){
        lFrom = gAllZAxisFromThreeQuarters;
    }
    if(lFrom==0&&pValue>=.5){
        lFrom = gAllZAxisFromHalf;
    }
    if(lFrom==0&&pValue>=.25){
        lFrom = gAllZAxisFromQuarter;
    }
    if(lFrom==0){
        lFrom = gAllZAxisFrom0;
    }

    ZAxis *lLast = 0;
    while(lFrom!=0){
        if(lFrom->getValue()>=pValue){
            lFrom->insertObject(lNewAxis);
            updateCollections(lNewAxis);
            return lNewAxis;
        }
        lLast = lFrom;
        lFrom = lFrom->getNext();
    }
    lLast->addObject(lNewAxis);
    updateCollections(lNewAxis);
    return lNewAxis;
}

AxisManager::~AxisManager(){
    ZAxis *lAxis = gAllZAxisFrom0;
    while(lAxis!=0){
        ZAxis *bNext = lAxis->getNext();
        delete lAxis;
        lAxis = bNext;
    }
}

void AxisManager::addNewObject(IGraphicsObject *pObject){
    obtainZAxisObject(pObject->getZ())->obtainYAxisObject(pObject->getY())->obtainXAxisObject(pObject->getX())->addGraphicsObject(pObject);
    updateCounts();
}

void AxisManager::drawFrame(PictureCache *pData){
    IGraphicsObject *lObject = getFirstGraphicsObject();
    while(lObject!=0){
        lObject->drawObject(pData);
        lObject = getNextGraphicsObject(lObject);
    }
}

IGraphicsObject *AxisManager::getFirstGraphicsObject(){
    IGraphicsObject *lObject = 0;
    ZAxis *lAxis = gAllZAxisFrom0;
    while(lAxis!=0){
        lObject = lAxis->getFirstGraphicsObject();
        if(lObject!=0){
            return lObject;
        }

        lAxis = lAxis->getNext();
    }
    return 0;
}

IGraphicsObject *AxisManager::getNextGraphicsObject(IGraphicsObject *pGraphicsObject){
    if(pGraphicsObject==0){
        return 0;
    }else if(!pGraphicsObject->getNext()==0){
        IGraphicsObject *lGraphicsObject = pGraphicsObject->getNext();
        if(lGraphicsObject==pGraphicsObject){
            std::cout << "getNextGraphics object is returning same object" << std::endl;
            exit(1);
        }
        return lGraphicsObject;
    }else{
        ZAxis *lZaxis = getZAxisObject(pGraphicsObject->getZ());
        if(lZaxis==0){
            std::cout<< "We should have a z axis of " << pGraphicsObject->getZ() << " but we don't!"<< std::endl;
            exit(1);
        }
        IGraphicsObject *lGraphicsObject = lZaxis->getNextGraphicsObject(pGraphicsObject);
        if(lGraphicsObject==pGraphicsObject){
            std::cout << "getNextGraphics object 2 is returning same object" << std::endl;
            exit(1);
        }

        return lGraphicsObject;


    }
}

ZAxis *AxisManager::getZAxisObject(double pValue){
    if(pValue>=1){
        ZAxis *lAxis = getZAxisFromCollection(gAllZAxisFrom1, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }
    if(pValue>=.75){
        ZAxis *lAxis = getZAxisFromCollection(gAllZAxisFromThreeQuarters, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }   
    if(pValue>=.5){
        ZAxis *lAxis = getZAxisFromCollection(gAllZAxisFromHalf, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    if(pValue>=.25){
        ZAxis *lAxis = getZAxisFromCollection(gAllZAxisFromQuarter, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    ZAxis *lAxis = getZAxisFromCollection(gAllZAxisFrom0, pValue);
    if(lAxis!=0){
        return lAxis;
    }
    return 0;
}

ZAxis *AxisManager::getZAxisFromCollection(ZAxis *pCollection, double pValue){
    if(pCollection==0){
        return 0;
    }
    if(pCollection->getValue()==pValue){
        return pCollection;
    }
    ZAxis *lAxis = pCollection->getNext();
    while(lAxis!=0){
        if(lAxis->getValue()==pValue){
            return lAxis;
        }
        lAxis = lAxis->getNext();
    }
    return 0;
}