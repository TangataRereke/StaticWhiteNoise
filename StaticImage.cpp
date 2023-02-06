#ifndef STATICIMAGE_H
    #define STATICIMAGE_H
    #include "StaticImage.h"
#endif


void StaticImage::compile(short unsigned pWidth, short unsigned pHeight, short unsigned pLength){
    ColourClass lClass;
    PictureCacheDraw lDraw(gFrontPicture);
/*     int RATE = 5;
    int lX = 0;
    int lY = 0;
    while(lX<pWidth&&lY<pHeight){
        lClass.setRGB(rand()%256, rand()%256, rand()%256);
        lDraw.setPenColour(lClass);
        short lWidth = RATE + lX;
        short lHeight = RATE + lY;
        lDraw.drawSquare(lX - rand()%5, lY-rand()%5, lWidth, lHeight);
        lX = lX + RATE;
        if(lX>=pWidth){
            lX = 0;
            if(lY+RATE>=pHeight){
                break;
            }
            lY = lY + RATE;
        }


    } */
    for(short bX=0;bX<pWidth;bX++){
        for(short bY=0;bY<pHeight;bY++){
            lClass.setRGB(rand()%256, rand()%256, rand()%256);
            lDraw.setPenColour(lClass);
            lDraw.setPixel(bX, bY);
        }
    }

}

