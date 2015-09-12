//
// Created by john on 9/12/15.
//

#include "NeuralNetFactory.h"
#include "Perceptron.h"
#include "ValueNeuron.h"
#include <map>
#include <stdio.h>

NeuralNetwork* NeuralNetFactory::build(Chromosome *chromosome,
                                       int inputs,
                                       int outputs) {

    map<int, Neuron*> neuronMap;

    vector<Neuron*> neurons;
    vector<ValueNeuron*> inputNeurons;
    vector<Neuron*> outputNeurons;

    // Bias node
    ValueNeuron* biasNode = new ValueNeuron();
    biasNode->setValue(1);
    neuronMap.insert(make_pair(0, biasNode));
    neurons.push_back(biasNode);

    for(int i = 0; i < inputs; i++){
        ValueNeuron* inputNeuron = new ValueNeuron();

        inputNeurons.push_back(inputNeuron);
        neuronMap.insert(make_pair(i + 1, inputNeuron));
    }

    for(int i = 0; i < outputs; i++) {
        Perceptron* outputPerceptron = new Perceptron();

        outputNeurons.push_back(outputPerceptron);
        neurons.push_back(outputPerceptron);
        neuronMap.insert(make_pair(inputs + i + 1, outputPerceptron));
    }

    for(int i = 0; i < chromosome->getSize(); i++){
        Chromosome::Gene *gene = chromosome->getGene(i);
        if(neuronMap.find(gene->from) == neuronMap.end()){
            neuronMap.insert(make_pair(gene->from, new Perceptron()));
        }
        if(neuronMap.find(gene->to) == neuronMap.end()){
            neuronMap.insert(make_pair(gene->to, new Perceptron()));
        }

        if(gene->active){
            Neuron* targetNeuron = neuronMap.at(gene->to);
            Neuron* sourceNeuron = neuronMap.at(gene->from);
            targetNeuron->add(gene->weight, sourceNeuron);
        }
    }

    NeuralNetwork* network = new NeuralNetwork();
    network->setNeurons(neurons);
    network->setInputs(inputNeurons);
    network->setOutputs(outputNeurons);


    return network;
}
