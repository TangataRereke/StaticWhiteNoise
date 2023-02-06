#ifndef PICTURECACHE_H
	#define PICTURECACHE_H
	#include "PictureCache.h"
#endif
#include <math.h>
class PictureCacheDraw{
	public:
		PictureCacheDraw(PictureCache *pPictureCache){ gPictureCache = pPictureCache;}
		void drawLine(short pX, short pY, short pRight, short pBottom);
		void drawSquare(short pX, short pY, short pRight, short pBottom);
		void setPixel(short pX, short pY);
		void setPenColour(ColourClass pColour){memcpy(&gPenColour, &pColour.gColour, sizeof(ColourClass::ColourRef));};
		void drawCircle(short pMiddleX, short pMiddleY, short pRadius);
		void drawStar(short pMiddleX, short pMiddleY, short pRadius);
	private:
	protected:
		PictureCache *gPictureCache;
		ColourClass::ColourRef gPenColour;
};