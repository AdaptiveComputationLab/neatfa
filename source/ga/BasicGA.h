//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_BASICGA_H
#define IANT_ARGOS_BASICGA_H


#include "Chromosome.h"

class BasicGA {

public:

	BasicGA() :
    		TOTAL_NODE_COUNT(30),
    		ELITE_PERCENTAGE(0.20F),
    		WEIGHT_MUTATION_PERCENTAGE(0.50F),
    		EDGE_ADD_MUTATION_PERCENTAGE(0.20F),
    		EDGE_REMOVE_MUTATION_PERCENTAGE(0.10F),
    		NODE_ADD_MUTATION_PERCENTAGE(0.10F)
	{}

    struct FitnessChromosome{
        Real fitness;
        Chromosome* chromosome;

        bool operator<( const FitnessChromosome& that ) const {
            return fitness > that.fitness;
        }
    };

    vector<Chromosome*> evolve(vector<FitnessChromosome> input);

private:
    int TOTAL_NODE_COUNT;
    float ELITE_PERCENTAGE;
    float WEIGHT_MUTATION_PERCENTAGE;
    float EDGE_ADD_MUTATION_PERCENTAGE;
    float EDGE_REMOVE_MUTATION_PERCENTAGE;
    float NODE_ADD_MUTATION_PERCENTAGE;

    vector<BasicGA::FitnessChromosome> rouletteSelection(vector<BasicGA::FitnessChromosome> vector, int size);
    vector<Chromosome*> crossOver(vector<BasicGA::FitnessChromosome> input, int populationSize);
    vector<Chromosome*> mutation(vector<Chromosome*> input);
    vector<Chromosome*> elitism(vector<Chromosome*> someChromosomes, vector<BasicGA::FitnessChromosome> fitnessChromosomes);
    Chromosome::Gene *copy(Chromosome::Gene *input);
    float getRandomFloat(){return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));}
};


#endif //IANT_ARGOS_BASICGA_H
