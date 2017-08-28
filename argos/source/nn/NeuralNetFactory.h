//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_NEURALNETFACTORY_H
#define IANT_ARGOS_NEURALNETFACTORY_H


#include <source/ga/Chromosome.h>
#include "NeuralNetwork.h"

class NeuralNetFactory {

public:
    NeuralNetwork* build(Chromosome *chromosome,
                         int inputs,
                         int outputs);
};


#endif //IANT_ARGOS_NEURALNETFACTORY_H
