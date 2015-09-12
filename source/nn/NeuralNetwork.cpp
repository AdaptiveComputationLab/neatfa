//
// Created by john on 9/10/15.
//
#include "NeuralNetwork.h"

void NeuralNetwork::update() {
    random_shuffle(neurons.begin(), neurons.end());
    for(int i = 0; i < neurons.size(); i++){
        neurons.at(i)->update();
    }
}