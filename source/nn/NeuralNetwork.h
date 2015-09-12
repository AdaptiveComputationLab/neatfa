//
// Created by john on 9/10/15.
//

#ifndef IANT_ARGOS_NEURALNETWORK_H
#define IANT_ARGOS_NEURALNETWORK_H


#include "Neuron.h"
#include "ValueNeuron.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

class NeuralNetwork {

public:
    void update();
    void setNeurons(vector<Neuron*> inputNeurons){neurons = inputNeurons;}
    void setOutputs(vector<Neuron*> inputOutputs){outputs = inputOutputs;}
    vector<Neuron*> getOutputs(){ return outputs;}
    void setInputs(vector<ValueNeuron*> inputInputs){inputs = inputInputs;}
    vector<ValueNeuron*> getInputs(){ return inputs;}

private:
    vector<Neuron*> neurons;
    vector<ValueNeuron*> inputs;
    vector<Neuron*> outputs;

};


#endif //IANT_ARGOS_NEURALNETWORK_H
