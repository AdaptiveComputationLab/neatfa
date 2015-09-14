//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_BASICGA_H
#define IANT_ARGOS_BASICGA_H


#include "Chromosome.h"

class BasicGA {

public:
    struct FitnessChromosome{
        Real fitness;
        Chromosome* chromosome;

        bool operator<( const FitnessChromosome& that ) const {
            return fitness > that.fitness;
        }
    };

    vector<Chromosome*> evolve(vector<FitnessChromosome> input);

private:
    static const int TOTAL_NODE_COUNT = 30;
    static const float ELITE_PERCENTAGE = 0.30F;
    static const float WEIGHT_MUTATION_PERCENTAGE = 0.20F;
    static const float EDGE_ADD_MUTATION_PERCENTAGE = 0.20F;
    static const float EDGE_REMOVE_MUTATION_PERCENTAGE = 0.10F;
    static const float NODE_ADD_MUTATION_PERCENTAGE = 0.10F;

    vector<BasicGA::FitnessChromosome> rouletteSelection(vector<BasicGA::FitnessChromosome> vector, int size);
    vector<Chromosome*> crossOver(vector<BasicGA::FitnessChromosome> input, int populationSize);
    vector<Chromosome*> mutation(vector<Chromosome*> input);
    vector<Chromosome*> elitism(vector<Chromosome*> someChromosomes, vector<BasicGA::FitnessChromosome> fitnessChromosomes);
    Chromosome::Gene *copy(Chromosome::Gene *input);
    float getRandomFloat(){return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));}
};


#endif //IANT_ARGOS_BASICGA_H
