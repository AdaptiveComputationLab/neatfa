//
// Created by john on 9/12/15.
//

#include <stdlib.h>
#include <algorithm>
#include "BasicGA.h"
#include "Unique.h"

vector<Chromosome *> BasicGA::evolve(vector<BasicGA::FitnessChromosome> input) {

    int populationSize = input.size();

    //tournament selection
    vector<BasicGA::FitnessChromosome> tournamentChromosomes = rouletteSelection(input, populationSize);

    //crossover
    vector<Chromosome*> crossedOverChromosomes = crossOver(tournamentChromosomes, populationSize);

    //mutation
    vector<Chromosome*> mutatedChromosomes = mutation(crossedOverChromosomes);

    //elitism
    // copy sorted chromosomes after crossover and mutation
    vector<BasicGA::FitnessChromosome> sorted;
    sorted.assign(input.begin(), input.end());
    sort(sorted.begin(), sorted.end());
    return elitism(mutatedChromosomes, sorted);

    /*vector<Chromosome*> chromosomes;
    for(int i = 0; i < input.size(); i++){
        chromosomes.push_back(input.at(i).chromosome);
    }

    return chromosomes;*/
}

vector<BasicGA::FitnessChromosome> BasicGA::rouletteSelection(vector<BasicGA::FitnessChromosome> input, int populationSize) {
    vector<float> fitnesses;
    int fitnessSum = 0;
    for (int i = 0; i < input.size(); i++) {
        FitnessChromosome entry = input.at(i);
        fitnessSum += entry.fitness * entry.fitness;
        fitnesses.push_back(fitnessSum);
    }

    vector<float> normalizedFitnesses;
    for(int i = 0; i < fitnesses.size(); i++){
        if(fitnessSum == 0){
            normalizedFitnesses.push_back(1.0f);
        }
        else{
            normalizedFitnesses.push_back((fitnesses.at(i) - fitnesses.at(0)) / (fitnessSum - fitnesses.at(0)));
        }
    }

    vector<BasicGA::FitnessChromosome> outputChromosomes;
    for(int i = 0; i < populationSize; i++){
        float threshold = getRandomFloat();
        bool found = false;
        for(int j = 0; j < fitnesses.size() - 1; j++){
            if(fitnesses.at(j) < threshold && fitnesses.at(j + 1) > threshold){
                outputChromosomes.push_back(input.at(j + 1));
                found = true;
                break;
            }
        }
        if(!found){
            outputChromosomes.push_back(input.at(0));
        }
    }

    return outputChromosomes;
}

vector<Chromosome*> BasicGA::crossOver(vector<BasicGA::FitnessChromosome> input, int populationSize){
    vector<Chromosome*> outputChromosomes;

    for (int i=0; i < populationSize; i++){
        //Select candidates for crossover at random
        BasicGA::FitnessChromosome candidate1 = input.at(rand() % populationSize);
        BasicGA::FitnessChromosome candidate2 = input.at(rand() % populationSize);

        Chromosome* offSpring = new Chromosome();

        int maxFeature = max(candidate1.chromosome->getMaxFeature(), candidate2.chromosome->getMaxFeature());
        int minFeature = min(candidate1.chromosome->getMinFeature(), candidate2.chromosome->getMinFeature());

        //randomly choose parent chromosome feature to incorporate.
        for(int f = minFeature; f <= maxFeature; f++){
            Chromosome::Gene* gene1 = candidate1.chromosome->getGeneByFeature(f);
            Chromosome::Gene* gene2 = candidate2.chromosome->getGeneByFeature(f);

            if(rand() % 2 == 0){
                if(gene1 != NULL){
                    offSpring->addGene(copy(gene1));
                }
            }
            else{
                if(gene2 != NULL){
                    offSpring->addGene(copy(gene2));
                }
            }
        }

        outputChromosomes.push_back(offSpring);
    }
    return outputChromosomes;
}

vector<Chromosome*> BasicGA::elitism(vector<Chromosome*> someChromosomes, vector<BasicGA::FitnessChromosome> fitnessChromosomes) {
    vector<Chromosome*> elite;
    int eliteCandidateSize = someChromosomes.size() * ELITE_PERCENTAGE;
    for (int i = 0; i < someChromosomes.size(); i++) {
        if (i > eliteCandidateSize || fitnessChromosomes.at(i).fitness == 0) {
            elite.push_back(someChromosomes.at(i));
        } else {
            cout << "KEEPING: ";
            cout << fitnessChromosomes.at(i).fitness;
            cout << endl;
            elite.push_back(fitnessChromosomes.at(i).chromosome);

        }
    }

    return elite;
}

vector<Chromosome*> BasicGA::mutation(vector<Chromosome*> input) {
    // weight mutation
    for(int i = 0; i < input.size(); i++){
        Chromosome* candidate = input.at(i % input.size());

        if (getRandomFloat() <= WEIGHT_MUTATION_PERCENTAGE) {
            int edgesToMutate = (rand() % 4) + 1;
            cout << "WEIGHT MUTATION (" << edgesToMutate << "): ";
            for (int i = 0; i < edgesToMutate; i++) {
                Chromosome::Gene *genCandidate = candidate->getRandomGene();
                genCandidate->weight += (getRandomFloat() - 0.5f);
                cout << genCandidate->feature << ", ";
            }
            cout  << endl;
        }

        //edge add mutation
        if (getRandomFloat() <= EDGE_ADD_MUTATION_PERCENTAGE) {
            int edgesToAdd = 1;
            cout << "EDGE MUTATION (" << edgesToAdd << "): ";
            for(int k = 0; k < edgesToAdd; k++){
                int startNode = candidate->getRandomNode();
                int endNode = candidate->getRandomNode();

                bool buildEdge = true;
                // if connection exists and is disabled, enable
                for(int l = 0; l < candidate->getSize(); l++){
                    Chromosome::Gene* gene = candidate->getGene(l);
                    if (gene->from == startNode && gene->to == endNode){
                        if(!gene->active){
                            gene->active = true;
                        }
                        //connection exists
                        buildEdge = false;
                        cout << "reactivate: " << gene->feature << ", ";
                    }
                }

                if(buildEdge){
                    Chromosome::Gene* gene = new Chromosome::Gene();
                    gene->active = true;
                    gene->feature = Unique::getInstance().getFeatureId();
                    gene->from = startNode;
                    gene->to = endNode;
                    gene->weight = getRandomFloat();
                    candidate->addGene(gene);
                    cout << gene->feature << ", ";
                }
            }
            cout << endl;
        }

        //edge remove mutation
        if (getRandomFloat() <= EDGE_REMOVE_MUTATION_PERCENTAGE) {
            int edgesToRemove = 1;
            cout << "REMOVING RANDOM EDGES (" << edgesToRemove << "): ";
            for(int m = 0; m < edgesToRemove; m++){
                Chromosome::Gene* genCandidate = candidate->getRandomGene();
                genCandidate->active = false;
                cout << genCandidate->feature << ", ";
            }
            cout << endl;
        }

        //node add mutation
        if (getRandomFloat() <= NODE_ADD_MUTATION_PERCENTAGE) {
            int nodesToAdd = 1;
            cout << "NODE ADD MUTATION (" << nodesToAdd << "): ";
            for(int n = 0; n < nodesToAdd /*(&& candidate->getSize() < TOTAL_NODE_COUNT*/; n++){
                Chromosome::Gene* splitEdge = candidate->getRandomGene();
                if(splitEdge->active){
                    int new_node = Unique::getInstance().getNodeId();

                    Chromosome::Gene* one = new Chromosome::Gene();
                    one->active = true;
                    one->feature = Unique::getInstance().getFeatureId();
                    one->from = splitEdge->from;
                    one->to = new_node;
                    one->weight = splitEdge->weight;

                    Chromosome::Gene* two = new Chromosome::Gene();
                    two->active = true;
                    two->feature = Unique::getInstance().getFeatureId();
                    two->from = new_node;
                    two->to = splitEdge->to;
                    two->weight = splitEdge->weight;

                    candidate->addGene(one);
                    candidate->addGene(two);

                    splitEdge->active = false;

                    cout << one->feature << " & " << two->feature << " adding " << new_node << ", ";
                }
            }
            cout << endl;
        }
    }

    return input;
}

Chromosome::Gene* BasicGA::copy(Chromosome::Gene* input){
    Chromosome::Gene* gene = new Chromosome::Gene();
    gene->active = input->active;
    gene->feature = input->feature;
    gene->from = input->from;
    gene->to = input->to;
    gene->weight = input->weight;

    return gene;
}