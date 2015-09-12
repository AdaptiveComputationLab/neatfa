//
// Created by john on 9/10/15.
//

#ifndef IANT_ARGOS_NEURON_H
#define IANT_ARGOS_NEURON_H

#include <argos3/core/utility/datatypes/datatypes.h>

using namespace argos;

class Neuron {

public:
    virtual Real getValue() { return -1; }
    Real getCachedValue() {return value;}
    void update() {value = getValue();}
    virtual void add(Real weight, Neuron* neuron){}

private:
    Real value;
};


#endif //IANT_ARGOS_NEURON_H
