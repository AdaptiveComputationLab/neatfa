//
// Created by john on 9/10/15.
//

#ifndef IANT_ARGOS_PERCEPTRON_H
#define IANT_ARGOS_PERCEPTRON_H


#include "Neuron.h"
#include <cmath>
#include <vector>

using namespace std;

class Perceptron : public Neuron {

public:
    Real getValue();
    void add(Real weight, Neuron* neuron);

private:
    vector<Neuron*> inputs;
    vector<Real> weights;
};


#endif //IANT_ARGOS_PERCEPTRON_H
