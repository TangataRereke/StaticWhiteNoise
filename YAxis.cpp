#ifndef YAXIS_H
    #define YAXIS_H
    #include "YAxis.h"
#endif

XAxis *YAxis::obtainXAxisObject(double pValue){
    XAxis *lAxis = getXAxisObject(pValue);
    if(lAxis){
        return lAxis;
    }
    return addXAxisObject(pValue);
}

XAxis *YAxis::addXAxisObject(double pValue){
    XAxis *lNewAxis = new XAxis(pValue);
    if(gAllXAxisFrom0==0){
        gAllXAxisFrom0 = lNewAxis;
        if(pValue>=.25){
            gAllXAxisFromQuarter = lNewAxis;
            if(pValue>=.5){
                gAllXAxisFromHalf = lNewAxis;
                if(pValue>=.75){
                    gAllXAxisFromThreeQuarters = lNewAxis;
                    if(pValue>=1){
                        gAllXAxisFrom1 = lNewAxis;
                    }
                }
                
            }
        }
        return lNewAxis;
    }
    XAxis *lFrom = 0;
    if(pValue>=1){
        lFrom = gAllXAxisFrom1;
    }
    if(lFrom==0&&pValue>=.75){
        lFrom = gAllXAxisFromThreeQuarters;
    }
    if(lFrom==0&&pValue>=.5){
        lFrom = gAllXAxisFromHalf;
    }
    if(lFrom==0&&pValue>=.25){
        lFrom = gAllXAxisFromQuarter;
    }
    if(lFrom==0){
        lFrom = gAllXAxisFrom0;
    }

    XAxis *lLast = 0;
    while(lFrom!=0){
        if(lFrom->getValue()>=pValue){
            lFrom->insertXObject(lNewAxis);
            updateCollections(lNewAxis);
            return lNewAxis;
        }
        lLast = lFrom;
        lFrom = lFrom->getNext();
    }
    lLast->addXObject(lNewAxis);
    updateCollections(lNewAxis);
    return lNewAxis;
}

void YAxis::updateCollections(XAxis *pNew){
    if(gAllXAxisFrom0!=0&&gAllXAxisFrom0->getValue()>pNew->getValue()){
        gAllXAxisFrom0 = pNew;
    }
    if(pNew->getValue()>=.25&&(gAllXAxisFromQuarter==0||gAllXAxisFromQuarter->getValue()>pNew->getValue())){
        gAllXAxisFromQuarter = pNew;
    }
    if(pNew->getValue()>=.5&&(gAllXAxisFromHalf==0||gAllXAxisFromHalf->getValue()>pNew->getValue())){
        gAllXAxisFromHalf = pNew;
    }
    if(pNew->getValue()>=.75&&(gAllXAxisFromThreeQuarters==0||gAllXAxisFromThreeQuarters->getValue()>pNew->getValue())){
        gAllXAxisFromThreeQuarters = pNew;
    }
    if(pNew->getValue()>=1&&(gAllXAxisFrom1==0||gAllXAxisFrom1->getValue()>pNew->getValue())){
        gAllXAxisFrom1 = pNew;
    }
}


void YAxis::displayOids(){
    std::cout << "      Y Axis BEGIN " << gValue << "/" << this << std::endl;
    std::cout << "       - 000 = " << gAllXAxisFrom0 << std::endl;
    std::cout << "       - 1/4 = " << gAllXAxisFromQuarter << std::endl;
    std::cout << "       - 1/2 = " << gAllXAxisFromHalf << std::endl;
    std::cout << "       - 3/4 = " << gAllXAxisFromThreeQuarters<< std::endl;
    std::cout << "       - 001 = " << gAllXAxisFrom1 << std::endl;    
    XAxis *lAxis = gAllXAxisFrom0;
    while(lAxis!=0){
        lAxis->displayOids();
        lAxis = lAxis->getNext();
    }
    std::cout << "      Y Axis END " << gValue << "/" << this << std::endl;
}

void YAxis::insertObject(YAxis *pNewAxis){
    pNewAxis->gPrevious = gPrevious;
    pNewAxis->gNext = this;
    if(pNewAxis->gPrevious!=0){
        pNewAxis->gPrevious->gNext = pNewAxis;
    }
    gPrevious = pNewAxis;
}
void YAxis::addObject(YAxis *pNewAxis){
    pNewAxis->gPrevious = this;
    gNext = pNewAxis;
}



IGraphicsObject *YAxis::getNextGraphicsObject(IGraphicsObject *pGraphicsObject){
    if(pGraphicsObject==0){
        return 0;
    }else if(!pGraphicsObject->getNext()==0){
        return pGraphicsObject->getNext();
    }else{
        XAxis *lXaxis = getXAxisObject(pGraphicsObject->getX());
        if(lXaxis==0){
            std::cout<< "We should have a x axis of " << pGraphicsObject->getX() << " but we don't!"<< std::endl;
            exit(1);
        }
        IGraphicsObject *lGraphicsObject = lXaxis->getFirstGraphicsObject();
        if(lGraphicsObject==0){
            if(gNext==0){
                return  0;
            }
            YAxis *lYaxis = gNext;
            while(lYaxis!=0){
                lGraphicsObject = lYaxis->getFirstGraphicsObject();
                if(lGraphicsObject!=0){
                    return lGraphicsObject;
                }
                lYaxis = lYaxis->gNext;
            }
        }
        if(lGraphicsObject==pGraphicsObject){
            return 0;
        }
        if(lGraphicsObject==pGraphicsObject){
            std::cout << "pGraphics object i sth esame as as returned, in x axis. Exiting. " << std::endl;
            exit(1);
        }
        return lGraphicsObject;


    }
}

IGraphicsObject *YAxis::getFirstGraphicsObject(){
    XAxis *lXAxis = gAllXAxisFrom0;
    while(lXAxis!=0){
        IGraphicsObject *lGraphicsObject = lXAxis->getFirstGraphicsObject();
        if(lGraphicsObject!=0){
            return lGraphicsObject;
        }
        lXAxis = lXAxis->getNext();
    }
    return 0;
}

XAxis *YAxis::getXAxisObject(double pValue){
    if(pValue>=1){
        XAxis *lAxis = getXAxisFromCollection(gAllXAxisFrom1, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }
    if(pValue>=.75){
        XAxis *lAxis = getXAxisFromCollection(gAllXAxisFromThreeQuarters, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }   
    if(pValue>=.5){
        XAxis *lAxis = getXAxisFromCollection(gAllXAxisFromHalf, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    if(pValue>=.25){
        XAxis *lAxis = getXAxisFromCollection(gAllXAxisFromQuarter, pValue);
        if(lAxis!=0){
            return lAxis;
        }
    }        
    XAxis *lAxis = getXAxisFromCollection(gAllXAxisFrom0, pValue);
    if(lAxis!=0){
        return lAxis;
    }
    return 0;
}

XAxis *YAxis::getXAxisFromCollection(XAxis *pCollection, double pValue){
    if(pCollection==0){
        return 0;
    }
    if(pCollection->getValue()==pValue){
        return pCollection;
    }
    XAxis *lAxis = pCollection->getNext();
    while(lAxis!=0){
        if(lAxis->getValue()==pValue){
            return lAxis;
        }
        lAxis = lAxis->getNext();
    }
    return 0;
}

void YAxis::updateCounts(){
    gObjectCount = 0;
    XAxis *lAxis = gAllXAxisFrom0;
    list<XAxis*> lDeleteObjects;
    while(lAxis){
        if(lAxis->getObjectCount()==0){
            lDeleteObjects.push_back(lAxis);
        }else{
            gObjectCount = gObjectCount + lAxis->getObjectCount();
        }
        lAxis = lAxis->getNext();
    }
    deleteObjects(lDeleteObjects);
}

void YAxis::deleteObject(XAxis *pObject){
    if(pObject==gAllXAxisFrom0){
        gAllXAxisFrom0 = pObject->getNext();
    }
    if(pObject==gAllXAxisFromQuarter){
        gAllXAxisFromQuarter = pObject->getNext();
    }
    if(pObject==gAllXAxisFromHalf){
        gAllXAxisFromHalf = pObject->getNext();
    }
    if(pObject==gAllXAxisFromThreeQuarters){
        gAllXAxisFromThreeQuarters = pObject->getNext();
    }
    if(pObject==gAllXAxisFrom1){
        gAllXAxisFrom1 = pObject->getNext();
    }
    pObject->deleteXObject();
}

void YAxis::deleteObjects(list<XAxis*> pObjects){
    for(XAxis *lObject : pObjects){
        deleteObject(lObject);
    }
}

void YAxis::deleteObject(){
    if(gPrevious!=0){
        gPrevious->gNext = gNext;
    }
    if(gNext!=0){
        gNext->gPrevious = gPrevious;
    }
    delete this;
}

YAxis::~YAxis(){
    XAxis *lAxis = gAllXAxisFrom0;
    while(lAxis!=0){
        XAxis *bNext = lAxis->getNext();
        delete lAxis;
        lAxis = bNext;
    }
}