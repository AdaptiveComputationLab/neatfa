//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_CHROMOSOME_H
#define IANT_ARGOS_CHROMOSOME_H

#include <vector>
#include <set>
#include <argos3/core/utility/string_utilities.h>
#include <stdlib.h>

using namespace std;
using namespace argos;

class Chromosome {

public:

    struct Gene {
        int feature;
        int from;
        int to;
        Real weight;
        bool active;
    };

    Gene* getGene(int i){return genes.at(i);}
    Gene* getGeneByFeature(int feature);
    int getSize() {return genes.size();}
    Gene* getRandomGene(){return getGene(rand() % genes.size());}
    int getRandomNode();
    void addGene(Gene* gene);
    int getMaxFeature();
    int getMinFeature();

    void removeRandomGene();

private:
    vector<Gene*> genes;
    set<int> nodes;
    int id;
};


#endif //IANT_ARGOS_CHROMOSOME_H
