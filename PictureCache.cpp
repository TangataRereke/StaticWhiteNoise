#ifndef PICTURECACHE_H
    #define PICTURECACHE_H
    #include "PictureCache.h"
#endif
PictureCache::PictureCache(short unsigned pWidth, short unsigned pHeight){
    gWidth = pWidth;
    gHeight = pHeight;
    gSize = (pWidth * pHeight * 3) + 1;
    gData = new BYTE[gSize];
    memset(gData, 0, gSize);
}
PictureCache::~PictureCache(){
    if(gData!=0){
        delete gData;
    }
}

bool PictureCache::isInLimits(){
    if(gLeft==32000||gTop==32000){
        return true;
    }
    return (gLeft+gActualWidth)>=0&&gLeft<gWidth&&(gTop+gActualHeight)>=0&&gTop<gHeight;
}

bool PictureCache::setPixel(short pX, short pY, ColourClass::ColourRef pColour){
    if(pX<0){
        return false;
    }
    if(pY<0){
        return false;
    }
    if(pX>=gWidth){
        return false;
    }
    if(pY>=gHeight){
        return false;
    }
        
    if(pY>gActualHeight){
        gActualHeight = pY;
    }
    if(pX>gActualWidth){
        gActualWidth = pX;
    }

    memcpy(gData+getStartBuffer(pX, pY), &pColour, sizeof(ColourClass::ColourRef));
    return true;
}

void PictureCache::copyTo(BYTE *pData){
    memcpy(pData, gData, gSize);
}

void PictureCache::copyTo(PictureCache *pPictureCache){
    if((gLeft+gActualWidth)<0||(gTop+gActualHeight)<0){
        return;
    }
    if(gLeft>=(pPictureCache->gWidth-1)){
        return;
    }

    for(short unsigned lY = 0; lY <= gActualHeight; lY++){
        if(lY+gTop<0){
            continue;
        }
        if(lY+gTop>=pPictureCache->gHeight){
            break;
        }
        long long lHereOffset = lY * gWidth;
        if(gLeft<0){
            lHereOffset = lHereOffset + -gLeft;
        }
        long long lThereOffset = ((lY + gTop) * pPictureCache->gWidth) + max(gLeft,(short)0);
        short unsigned lUseWidth = gActualWidth;
        if(gLeft + gActualWidth>pPictureCache->gWidth){
            lUseWidth = (pPictureCache->gWidth - gLeft);
        }
        if(gLeft<0){
            lUseWidth = lUseWidth + gLeft;
        }
        if(lUseWidth<=0){
            continue;;
        }
        lHereOffset = lHereOffset * 3;
        lThereOffset = lThereOffset * 3;
        if(gLeft+lUseWidth>pPictureCache->gWidth){
            std::cout << "We still have a probem here " << gLeft << "<" << lUseWidth<< ">" << lUseWidth << std::endl;
            exit(1);
        }else if(lThereOffset>pPictureCache->gSize){
            std::cout << "There position is going to be too large, exiting: lThereOffset: " << lThereOffset << ", Use Width: " << lUseWidth << " added together: " << lThereOffset + lUseWidth << " and total picture size " << pPictureCache->gSize << std::endl;
            exit(1);
        }else if((lHereOffset)>gSize){
            std::cout << "The here and use width is too large, exiting: lHereOffset: " << lHereOffset << ", use width: " << lUseWidth << ", added together: " << lHereOffset + lUseWidth << ", and the size: " << gSize << std::endl;
            exit(1);
        }else if(lThereOffset<0){
            std::cout << "there offset is too low " << lThereOffset << std::endl;
            exit(1);
        }else if(lHereOffset<0){
            std::cout << "here offset is too low " << lHereOffset << std::endl;
            exit(1);

        }
        lUseWidth = (lUseWidth+1) * 3;
        memcpy(pPictureCache->gData+lThereOffset, gData+lHereOffset, lUseWidth);
    }
}

inline unsigned int PictureCache::getStartBuffer(const short unsigned pX, const short unsigned pY){
    return ((pY * gWidth) + pX) * sizeof(ColourClass::ColourRef);
}
