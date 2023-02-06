#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif
#ifndef XAXIS_H
    #define XAXIS_H
    #include "XAxis.h"
#endif
#include <list>
class IGraphicsObject;
class YAxis{
    public:
        void displayOids();
        double getValue(){return gValue;}
        YAxis *getNext(){return gNext;}
        YAxis(double pValue){ gValue = pValue;}
        void insertObject(YAxis *pNewAxis);
        void addObject(YAxis *pNewAxis);
        XAxis *obtainXAxisObject(double pValue);
        IGraphicsObject *getFirstGraphicsObject();
        IGraphicsObject *getNextGraphicsObject(IGraphicsObject *pGraphicsObject);
        void updateCounts();
        long long unsigned getObjectCount(){return gObjectCount;};
        void deleteObject();
        XAxis *getXAxisObject(double pValue);    
        XAxis *addXAxisObject(double pValue);
        ~YAxis();
    protected:
    private:
        void deleteObjects(list<XAxis*> pObjects);
        void deleteObject(XAxis *pObject);
        XAxis *getXAxisFromCollection(XAxis *pCollection, double pValue);
        double gValue = 0;
        YAxis *gNext = 0;
        YAxis *gPrevious = 0;
        void updateCollections(XAxis *pNew);
        long long unsigned gObjectCount = 0;
        XAxis *gAllXAxisFrom0 = 0;
        XAxis *gAllXAxisFromQuarter = 0;
        XAxis *gAllXAxisFromHalf = 0;
        XAxis *gAllXAxisFromThreeQuarters = 0;
        XAxis *gAllXAxisFrom1 = 0;
};