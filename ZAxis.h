#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif
#ifndef YAXIS_H
    #define YAXIS_H
    #include "YAxis.h"
#endif
class ZAxis{
    public:
        ZAxis(double pValue){ gValue = pValue;};
        void displayOids();    
        double getValue(){return gValue;}
        ZAxis *getNext(){return gNext;}
        void insertObject(ZAxis *pNewAxis);
        void addObject(ZAxis *pNewAxis);
        YAxis *obtainYAxisObject(double pValue);
        IGraphicsObject *getNextGraphicsObject(IGraphicsObject *pGraphicsObject);
        IGraphicsObject *getFirstGraphicsObject();
        ZAxis *getPrevious(){return gPrevious;};
        long long unsigned getObjectCount(){return gObjectCount;};
        void updateCounts();
        ~ZAxis();
        void deleteObject();
        YAxis *getYAxisObject(double pValue);    
        YAxis *addYAxisObject(double pValue);
    protected:
    private:
        void deleteObjects(list<YAxis*> pObjects);
        void deleteObject(YAxis *pObject);
        YAxis *getYAxisFromCollection(YAxis *pCollection, double pValue);
        double gValue = 0;
        void updateCollections(YAxis *pNew);

        long long unsigned gObjectCount = 0;
        ZAxis *gNext = 0;
        ZAxis *gPrevious = 0;

        YAxis *gAllYAxisFrom0 = 0;
        YAxis *gAllYAxisFromQuarter = 0;
        YAxis *gAllYAxisFromHalf = 0;
        YAxis *gAllYAxisFromThreeQuarters = 0;
        YAxis *gAllYAxisFrom1 = 0;
};