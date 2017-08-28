//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_UNIQUE_H
#define IANT_ARGOS_UNIQUE_H


#include <stdlib.h>


class Unique {
public:
    static Unique& getInstance()
    {
        static Unique instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }

    int getNodeId(){ return ++atomicId;}
    void setNodeId(int nodeId){ atomicId = nodeId;}
    int getFeatureId(){ return ++featureId;}
private:
    Unique() {};                   // Constructor? (the {} brackets) are needed here.

    int atomicId;
    int featureId;

};


#endif //IANT_ARGOS_UNIQUE_H
