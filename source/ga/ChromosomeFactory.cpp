//
// Created by john on 9/13/15.
//

#include "ChromosomeFactory.h"
#include "Unique.h"

vector<Chromosome *> ChromosomeFactory::buildPopulation(int count) {
    vector<Chromosome *> population;

    for(int c = 0; c < count; c++){
        Chromosome* chromosome = new Chromosome();

        for(int i = 0; i < INPUT_COUNT; i++){
            for(int j = 0; j < OUTPUT_COUNT; j++){
                Chromosome::Gene *gene = new Chromosome::Gene();
                gene->feature = Unique::getInstance().getFeatureId();
                gene->active = true;
                gene->from = i;
                gene->to = INPUT_COUNT + j;
                gene->weight = (2 * getRandomFloat()) - 1; //random weights
                chromosome->addGene(gene);
            }
        }
        population.push_back(chromosome);
    }

    return population;
}
