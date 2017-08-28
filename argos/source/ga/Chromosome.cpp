//
// Created by john on 9/12/15.
//

#include "Chromosome.h"

int Chromosome::getMaxFeature() {
    int max = 0;

    for(int i = 0 ; i < genes.size(); i++){
        if(max < genes.at(i)->feature){
            max = genes.at(i)->feature;
        }
    }
    return max;
}

int Chromosome::getMinFeature() {
    int min = 0;

    for(int i = 0 ; i < genes.size(); i++){
        if(min > genes.at(i)->feature){
            min = genes.at(i)->feature;
        }
    }
    return min;
}

void Chromosome::addGene(Chromosome::Gene *gene) {
    genes.push_back(gene);
    nodes.insert(gene->to);
    nodes.insert(gene->from);
}

int Chromosome::getRandomNode() {
    set<int>::const_iterator it(nodes.begin());
    advance(it, rand() % nodes.size());

    return *it;
}

Chromosome::Gene *Chromosome::getGeneByFeature(int feature) {
    for(int i = 0; i < genes.size(); i++){
        if(genes.at(i)->feature == feature){
            return genes.at(i);
        }
    }

    return NULL;
}

void Chromosome::removeRandomGene() {
    genes.erase(genes.begin() + (rand() % genes.size()));
    nodes.clear();
    for(int i = 0; i < genes.size(); i++){
        Gene* gene = genes.at(i);
        nodes.insert(gene->to);
        nodes.insert(gene->from);
    }
}
