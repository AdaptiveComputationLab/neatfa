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
    static const int PROXIMITY_COUNT = 4;
    static const int HOLDING_FOOD_COUNT = 1;
    static const int NEAR_FOOD_COUNT = 1;
    static const int LIGHT_SENSOR_COUNT = 4;
    static const int PHEROMONE_INPUT = 1;
    static const int INPUT_COUNT = BIAS_COUNT + COMPASS_COUNT + PROXIMITY_COUNT + HOLDING_FOOD_COUNT + NEAR_FOOD_COUNT + LIGHT_SENSOR_COUNT + PHEROMONE_INPUT;
    static const int WHEEL_OUTPUT = 2;
    static const int PHEROMONE_OUTPUT = 1;
    static const int OUTPUT_COUNT = WHEEL_OUTPUT + PHEROMONE_OUTPUT;

private:
    float getRandomFloat(){return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));}
    Chromosome::Gene *copy(Chromosome::Gene *input);
};


#endif //IANT_ARGOS_CHROMOSOMEFACTORY_H
