#ifndef ZAXIS_H
    #define ZAXIS_H
    #include "ZAxis.h"
#endif
#ifndef IGRAPHICSOBJECT_H
    #define IGRAPHICSOBJECT_H
    #include "IGraphicsObject.h"
#endif

#include <list>
class AxisManager{
    public:
        IGraphicsObject *getFirstGraphicsObject();
        IGraphicsObject *getNextGraphicsObject(IGraphicsObject *pGraphicsObject);
        bool deleteObjects();
        bool moveObjects();
        void addMoveObject(IGraphicsObject *pObject);
        void drawFrame(PictureCache *pData);
        void addDeleteObject(IGraphicsObject *pObject);
        void displayOids(const char *pName);
        void addNewObject(IGraphicsObject *pObject);
        long long unsigned getObjectCount(){ return gObjectCount;}
        void updateCounts();
        ~AxisManager();
    protected:
    private:
        void deleteObject(ZAxis *pObject);
        void deleteObjects(list<ZAxis*> pObjects);

        ZAxis *getZAxisFromCollection(ZAxis *pCollection, double pValue);
        ZAxis *addZAxisObject(double pValue);
        ZAxis *obtainZAxisObject(double pValue);
        ZAxis *getZAxisObject(double pValue);
        ZAxis *gAllZAxisFrom0 = 0;
        ZAxis *gAllZAxisFromQuarter = 0;
        ZAxis *gAllZAxisFromHalf = 0;
        ZAxis *gAllZAxisFromThreeQuarters = 0;
        ZAxis *gAllZAxisFrom1 = 0;
        void updateCollections(ZAxis *pNew);
        void updateMoveObject(IGraphicsObject *pObject);
        list<IGraphicsObject*> gAllObjectsToMove;
        list<IGraphicsObject*> gAllObjectsToDelete;
        long long unsigned gObjectCount = 0;
};