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