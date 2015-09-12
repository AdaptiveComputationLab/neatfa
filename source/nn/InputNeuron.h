//
// Created by john on 9/10/15.
//

#ifndef IANT_ARGOS_INPUTNEURON_H
#define IANT_ARGOS_INPUTNEURON_H


#include "Neuron.h"

class InputNeuron : public Neuron {

public:
    void setValue(Real input) {value = input;}
    virtual Real getValue() {return value;}

private:
    Real value;
};


#endif //IANT_ARGOS_INPUTNEURON_H
