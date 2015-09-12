//
// Created by john on 9/10/15.
//

#ifndef IANT_ARGOS_NEURALNETWORK_H
#define IANT_ARGOS_NEURALNETWORK_H


#include "Neuron.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

class NeuralNetwork {

public:
    void update();
    void add(Neuron &neuron);

private:
    vector<Neuron*> neurons;

};


#endif //IANT_ARGOS_NEURALNETWORK_H
