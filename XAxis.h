#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif
class IGraphicsObject;
class XAxis{
    public:
        XAxis(double pValue){gValue = pValue;};
        void displayOids();
        IGraphicsObject *getNextGraphicsObject(IGraphicsObject *pGraphicsObject);
        IGraphicsObject *getFirstGraphicsObject(){ return gAllGraphicsObjects;}
        double getValue(){ return gValue;}
        XAxis *getNext(){return gNext;}
        void insertXObject(XAxis *pNewAxis);
        void addXObject(XAxis *pNewAxis);
        void deleteXObject();
        void addGraphicsObject(IGraphicsObject *pObject);
        void removeGraphicsObject(IGraphicsObject *pObject);
        long long unsigned getObjectCount(){ return gObjectCount;}
        ~XAxis();
    private:
    protected:
        IGraphicsObject *gAllGraphicsObjects = 0;
        IGraphicsObject *gLastAddedGraphicsObject = 0;
        double gValue = 0;
        XAxis *gNext = 0;
        XAxis *gPrevious = 0;
        long long unsigned gObjectCount = 0;
};