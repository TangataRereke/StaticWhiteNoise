#ifndef ZAXIS_H
    #define ZAXIS_H
    #include "ZAxis.h"
#endif

void ZAxis::updateCollections(YAxis *pNew){
    if(gAllYAxisFrom0!=0&&gAllYAxisFrom0->getValue()>pNew->getValue()){
        gAllYAxisFrom0 = pNew;
    }
    if(pNew->getValue()>=.25&&(gAllYAxisFromQuarter==0||gAllYAxisFromQuarter->getValue()>pNew->getValue())){
        gAllYAxisFromQuarter = pNew;
    }
    if(pNew->getValue()>=.5&&(gAllYAxisFromHalf==0||gAllYAxisFromHalf->getValue()>pNew->getValue())){
        gAllYAxisFromHalf = pNew;
    }
    if(pNew->getValue()>=.75&&(gAllYAxisFromThreeQuarters==0||gAllYAxisFromThreeQuarters->getValue()>pNew->getValue())){
        gAllYAxisFromThreeQuarters = pNew;
    }
    if(pNew->getValue()>=1&&(gAllYAxisFrom1==0||gAllYAxisFrom1->getValue()>pNew->getValue())){
        gAllYAxisFrom1 = pNew;
    }
}

YAxis *ZAxis::addYAxisObject(double pValue){
    YAxis *lNewAxis = new YAxis(pValue);
    if(gAllYAxisFrom0==0){
        gAllYAxisFrom0 = lNewAxis;
        if(pValue>=.25){
            gAllYAxisFromQuarter = lNewAxis;
            if(pValue>=.5){
                gAllYAxisFromHalf = lNewAxis;
                if(pValue>=.75){
                    gAllYAxisFromThreeQuarters = lNewAxis;
                    if(pValue>=1){
                        gAllYAxisFrom1 = lNewAxis;
                    }
                }
                
            }
        }
        return lNewAxis;
    }
    YAxis *lFrom = 0;
    if(pValue>=1){
        lFrom = gAllYAxisFrom1;
    }
    if(lFrom==0&&pValue>=.75){
        lFrom = gAllYAxisFromThreeQuarters;
    }
    if(lFrom==0&&pValue>=.5){
        lFrom = gAllYAxisFromHalf;
    }
    if(lFrom==0&&pValue>=.25){
        lFrom = gAllYAxisFromQuarter;
    }
    if(lFrom==0){
        lFrom = gAllYAxisFrom0;
    }

    YAxis *lLast = 0;
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

YAxis *ZAxis::getYAxisObject(double pValue){
    if(pValue>=1){
        YAxis *lAxis = getYAxisFromCollection(gAllYAxisFrom1, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }
    if(pValue>=.75){
        YAxis *lAxis = getYAxisFromCollection(gAllYAxisFromThreeQuarters, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }   
    if(pValue>=.5){
        YAxis *lAxis = getYAxisFromCollection(gAllYAxisFromHalf, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    if(pValue>=.25){
        YAxis *lAxis = getYAxisFromCollection(gAllYAxisFromQuarter, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    YAxis *lAxis = getYAxisFromCollection(gAllYAxisFrom0, pValue);
    if(lAxis!=0){
        return lAxis;
    }
    return 0;
}

YAxis *ZAxis::obtainYAxisObject(double pValue){
    YAxis *lAxis = getYAxisObject(pValue);
    if(lAxis){
        return lAxis;
    }
    return addYAxisObject(pValue);

    
}

void ZAxis::deleteObject(){
    if(gPrevious!=0){
        gPrevious->gNext = gNext;
    }
    if(gNext!=0){
        gNext->gPrevious = gPrevious;
    }
    delete this;
}


void ZAxis::deleteObject(YAxis *pObject){
    if(pObject==gAllYAxisFrom0){
        gAllYAxisFrom0 = pObject->getNext();
    }
    if(pObject==gAllYAxisFromQuarter){
        gAllYAxisFromQuarter = pObject->getNext();
    }
    if(pObject==gAllYAxisFromHalf){
        gAllYAxisFromHalf = pObject->getNext();
    }
    if(pObject==gAllYAxisFromThreeQuarters){
        gAllYAxisFromThreeQuarters = pObject->getNext();
    }
    if(pObject==gAllYAxisFrom1){
        gAllYAxisFrom1 = pObject->getNext();
    }
    pObject->deleteObject();
}

void ZAxis::deleteObjects(list<YAxis*> pObjects){
    for(YAxis *lObject : pObjects){
        deleteObject(lObject);
    }
}

void ZAxis::displayOids(){
    std::cout << "   Z Axis BEGIN " << gValue << "/" << this << std::endl;
    std::cout << "   - 000 = " << gAllYAxisFrom0 << std::endl;
    std::cout << "   - 1/4 = " << gAllYAxisFromQuarter << std::endl;
    std::cout << "   - 1/2 = " << gAllYAxisFromHalf << std::endl;
    std::cout << "   - 3/4 = " << gAllYAxisFromThreeQuarters<< std::endl;
    std::cout << "   - 001 = " << gAllYAxisFrom1 << std::endl;
    YAxis *lAxis = gAllYAxisFrom0;
    while(lAxis!=0){
        lAxis->displayOids();
        lAxis = lAxis->getNext();
    }
    std::cout << "   Z Axis END" << gValue << "/" << this << std::endl;
    if(gNext!=0){
        gNext->displayOids();
    }
}

void ZAxis::insertObject(ZAxis *pNewAxis){
    pNewAxis->gPrevious = gPrevious;
    pNewAxis->gNext = this;
    if(pNewAxis->gPrevious!=0){
        pNewAxis->gPrevious->gNext = pNewAxis;
    }
    gPrevious = pNewAxis;
}
void ZAxis::addObject(ZAxis *pNewAxis){
    pNewAxis->gPrevious = this;
    gNext = pNewAxis;
}

void ZAxis::updateCounts(){
    gObjectCount = 0;
    YAxis *lAxis = gAllYAxisFrom0;
    list<YAxis*> lDeleteObjects;
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


IGraphicsObject *ZAxis::getNextGraphicsObject(IGraphicsObject *pGraphicsObject){
    if(pGraphicsObject==0){
        return 0;
    }else if(pGraphicsObject->getNext()!=0){
        if(pGraphicsObject->getNext()==pGraphicsObject){
            std::cout << "Graphics and next is the same in zaxis get next so exiting" << std::endl;
            exit(1);
        }
        return pGraphicsObject->getNext();
    }else{
        YAxis *lYaxis = getYAxisObject(pGraphicsObject->getY());
        if(lYaxis==0){
            std::cout<< "We should have a y axis of " << pGraphicsObject->getY() << " but we don't!"<< std::endl;
            exit(1);
        }
        IGraphicsObject *lGraphicsObject = lYaxis->getNextGraphicsObject(pGraphicsObject);
        if(lGraphicsObject==0){
            if(gNext==0){
                return 0;
            }
            ZAxis *lZAxis = gNext;
            while(lZAxis!=0){
                lGraphicsObject = lZAxis->getFirstGraphicsObject();
                if(lGraphicsObject!=0){
                    return lGraphicsObject;
                }
                lZAxis = lZAxis->gNext;
            }
        }
        if(lGraphicsObject==pGraphicsObject){
            std::cout << "Next returning same for YAxis, exiting" << std::endl;
            exit(1);
        }
        return lGraphicsObject;


    }
}

IGraphicsObject *ZAxis::getFirstGraphicsObject(){
    YAxis *lYAxis = gAllYAxisFrom0;
    while(lYAxis!=0){
        IGraphicsObject *lGraphicsObject = lYAxis->getFirstGraphicsObject();
        if(lGraphicsObject!=0){
            return lGraphicsObject;
        }
        lYAxis = lYAxis->getNext();
    }
    return 0;
}

YAxis *ZAxis::getYAxisFromCollection(YAxis *pCollection, double pValue){
    if(pCollection==0){
        return 0;
    }
    if(pCollection->getValue()==pValue){
        return pCollection;
    }
    YAxis *lAxis = pCollection->getNext();
    while(lAxis!=0){
        if(lAxis->getValue()==pValue){
            return lAxis;
        }
        lAxis = lAxis->getNext();
    }
    return 0;
}

ZAxis::~ZAxis(){
    YAxis *lAxis = gAllYAxisFrom0;
    while(lAxis!=0){
        YAxis *bNext = lAxis->getNext();
        delete lAxis;
        lAxis = bNext;
    }
}