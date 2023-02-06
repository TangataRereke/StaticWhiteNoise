#include <memory.h>
#include <sstream>
#include <iostream>

using namespace std;

class ColourClass{
    public:
        struct ColourRef {
            char unsigned red;
            char unsigned green;
            char unsigned blue;
        };
        double getBrightness(){ return gBrightness;};
        const char *getDisplay();
        short getMax();
        short getMin();
        ColourRef gColour;
        void setRGB(short pRed, short pGreen, short pBlue);
        void setInteger(long unsigned pColour);
        void setHue(short unsigned pHue);
        short unsigned getHue(){return gHue;};
        void setSaturation(double pValue);
        double getSaturation(){return gSaturation;};
        void setBrightness(double pBrightness);
    private:
        void setRatiosBasedOffRGB();
        short unsigned getCalculatedHue();
        short unsigned gHue = 0;
        double getMaxRange();
        double getMidRange();
        double getMinRange();
        double gPercentRed = 0;
        double gPercentGreen = 0;
        double gPercentBlue = 0;
        double gOriginalRed = 0;
        double gOriginalGreen = 0;
        double gOriginalBlue = 0;
        double gSaturation = 0;
        double gBrightness = 0;
};