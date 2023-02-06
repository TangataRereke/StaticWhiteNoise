g++ -march=native -mtune=native -std=c++20 main.cpp Formula.cpp IBackground.cpp SoundManager.cpp SoundSequence.cpp ./IndependentFiles/Objects/ColourClass.o ./IndependentFiles/Objects/FramesCreator.o PictureCacheDraw.cpp FrameCreator.cpp IFrameGenerator.cpp PictureCache.cpp IGraphicsObject.cpp AxisManager.cpp ZAxis.cpp YAxis.cpp XAxis.cpp StaticImage.cpp FrameGenerator_Static.cpp  -Iffmpeg/include -Lffmpeg/lib -lavcodec -lavformat -lavutil  -lswresample -lswscale -D__STDC_CONSTANT_MACROS -o StaticWhiteNoiseMaker
chmod +x StaticWhiteNoiseMaker
