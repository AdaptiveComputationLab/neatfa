//
// Created by john on 9/10/15.
//

#include <stdio.h>
#include "Perceptron.h"

Real Perceptron::getValue() {
    Real sum = 0;

    for (unsigned long i = 0 ; i < inputs.size(); i++) {
        sum += inputs.at(i)->getCachedValue() * weights.at(i);
    }

    // Sigmoid
    return (float)((2 * (1 / (1 + exp(-sum)))) - 1);
}

void Perceptron::add(Real weight, Neuron* neuron) {
    inputs.push_back(neuron);
    weights.push_back(weight);
}
