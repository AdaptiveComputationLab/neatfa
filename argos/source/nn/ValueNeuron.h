//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_VALUENEURON_H
#define IANT_ARGOS_VALUENEURON_H


#include "Neuron.h"

class ValueNeuron : public Neuron{

public:
    Real getValue(){ return value;}
    void setValue(Real input){value = input;}
    void add(Real weight, Neuron* neuron){}

private:
    Real value;
};


#endif //IANT_ARGOS_VALUENEURON_H
