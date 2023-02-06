#ifndef PICTURECACHEDRAW_H
	#define PICTURECACHEDRAW_H
	#include "PictureCacheDraw.h"
#endif

void PictureCacheDraw::drawLine(short pX, short pY, short pRight, short pBottom){
    if(pY==pBottom){
        drawSquare(pX, pY, pRight, pBottom);
		return;
    }else if(pX==pRight){
        drawSquare(pX, pY, pRight, pBottom);
		return;
    }
    std::cout << "Draw line diagnals not implemented" << std::endl;
    exit(1);
}

void PictureCacheDraw::drawSquare(short pX, short pY, short pRight, short pBottom){
    for(short bY = pY; bY <= pBottom; bY++){
        for(short unsigned bX = pX; bX <= pRight; bX++){
            setPixel(bX, bY);
        }
    }
    return;
}

void PictureCacheDraw::setPixel(short pX, short pY){
    gPictureCache->setPixel(pX, pY, gPenColour);
}

void PictureCacheDraw::drawCircle(short pMiddleX, short pMiddleY, short pRadius){
    bool lRightRound = false;
    short lX = -pRadius;
    short lY = 0;
    short lPlotX = -pRadius;
    short lPlotY = 0;
    bool gUp = true;
    while(!lRightRound){
        setPixel(lPlotX + pMiddleX, lPlotY + pMiddleY);
        setPixel(pMiddleX - lPlotX , lPlotY + pMiddleY);
        if(abs(lPlotY)==pRadius){
            drawLine(pMiddleX - lPlotX, lPlotY + pMiddleY, lPlotX + pMiddleX, lPlotY + pMiddleY);
        }
        if(gUp){
            lY = lY - 1;
            if(lY<pRadius*-1){
                gUp = false;
                lY = lY + 1;
            }
        }else{
            lY = lY + 1;
            if(lY>pRadius){
                return;
            }
        }
        lPlotY = lY;

        double lValue = (sqrt(abs(lY)) / sqrt(pRadius)) * pRadius;
        lPlotX = pRadius - lValue;
    }

/*     The cool formula for(short unsigned lRadiusPoint = 0; lRadiusPoint < pRadius; lRadiusPoint++){
        short unsigned lX = (pRadius * cos(lRadiusPoint * M_PI / 180) + pMiddleX);
        short unsigned lY = (pRadius * sin(lRadiusPoint * M_PI / 180) + pMiddleX);
        setPixel(lX, lY);
    } */
}

void PictureCacheDraw::drawStar(short pMiddleX, short pMiddleY, short pRadius){
    bool lRightRound = false;
    short lX = -pRadius;
    short lY = 0;
    short lPlotX = -pRadius;
    short lPlotY = 0;
    bool gUp = true;
    while(!lRightRound){
        setPixel(lPlotX + pMiddleX, lPlotY + pMiddleY);
        setPixel(pMiddleX - lPlotX , lPlotY + pMiddleY);
        if(abs(lPlotY)==pRadius){
            drawLine(pMiddleX - lPlotX, lPlotY + pMiddleY, lPlotX + pMiddleX, lPlotY + pMiddleY);
        }
        if(gUp){
            lY = lY - 1;
            if(lY<pRadius*-1){
                gUp = false;
                lY = lY + 1;
            }
        }else{
            lY = lY + 1;
            if(lY>pRadius){
                return;
            }
        }
        lPlotY = lY;

        double lValue = (sqrt(abs(lY)) / sqrt(pRadius)) * pRadius;
        lPlotX = pRadius - lValue;
    }
}