//
// Created by john on 9/13/15.
//

#ifndef IANT_ARGOS_CHROMOSOMEFACTORY_H
#define IANT_ARGOS_CHROMOSOMEFACTORY_H


#include "Chromosome.h"

class ChromosomeFactory {

public:
    vector<Chromosome*> buildPopulation(int count);

    static const int BIAS_COUNT = 1;
    static const int COMPASS_COUNT = 4;
    static const int PROXIMITY_COUNT = 24;
    static const int HOLDING_FOOD_COUNT = 1;
    static const int NEAR_FOOD_COUNT = 1;
    static const int INPUT_COUNT = BIAS_COUNT + COMPASS_COUNT + PROXIMITY_COUNT + HOLDING_FOOD_COUNT + NEAR_FOOD_COUNT;
    static const int OUTPUT_COUNT = 2;

private:
    float getRandomFloat(){return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));}
    Chromosome::Gene *copy(Chromosome::Gene *input);
};


#endif //IANT_ARGOS_CHROMOSOMEFACTORY_H
