#ifndef IFRAMEGENERATOR_H
        #define IFRAMEGENERATOR_H
        #include "IFrameGenerator.h"
#endif
#ifndef STATICIMAGE_H
    #define STATICIMAGE_H
    #include "StaticImage.h"
#endif
#include <thread>
#include <vector>
class FrameGenerator_Static : public IFrameGenerator {
    public:
        FrameGenerator_Static(double pFactorX, double pFactorY, double pFactorZ, short unsigned pWidth, short unsigned pHeight) : IFrameGenerator(pFactorX, pFactorY, pFactorZ, pWidth, pHeight){};
        void prepareFrame();
        int getSoundPoint(int pPosition, int pChannel, int pTotalLength);
    protected:
        void prepareInitialFrame();
        void setupBackground();
    private:
        long long unsigned gSoundCutOff = 0;
        long long unsigned gSoundPosition = 0;
        double gCurrentSoundFrequency = 0;
        long long gCurrentAmplification = 20000;
        bool gIncreaseAmplification = false;
        long long unsigned gRate = 0;
    	double gCurrentZ = 0;
        StaticImage *gCurrentImage = 0;
};