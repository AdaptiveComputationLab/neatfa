//
// Created by john on 9/13/15.
//

#include "ChromosomeFactory.h"
#include "Unique.h"

vector<Chromosome *> ChromosomeFactory::buildPopulation(int count) {
    vector<Chromosome *> population;

    Chromosome* first = new Chromosome();

    for(int i = 0; i < INPUT_COUNT; i++){
        for(int j = 0; j < OUTPUT_COUNT; j++){
            Chromosome::Gene *gene = new Chromosome::Gene();
            gene->feature = Unique::getInstance().getFeatureId();
            gene->active = true;
            gene->from = i;
            gene->to = INPUT_COUNT + j + 1;
            gene->weight = 0;
            first->addGene(gene);
        }
    }
    population.push_back(first);

    for(int c = 0; c < count - 1; c++){
        Chromosome* chromosome = new Chromosome();

        for(int i = 0; i < first->getSize(); i++){
            chromosome->addGene(copy(first->getGene(i)));
        }

        population.push_back(chromosome);
    }

    Unique::getInstance().setNodeId(INPUT_COUNT + OUTPUT_COUNT + 1);

    return population;
}


Chromosome::Gene* ChromosomeFactory::copy(Chromosome::Gene* input){
    Chromosome::Gene* gene = new Chromosome::Gene();
    gene->active = input->active;
    gene->feature = input->feature;
    gene->from = input->from;
    gene->to = input->to;
    gene->weight = input->weight;

    return gene;
}