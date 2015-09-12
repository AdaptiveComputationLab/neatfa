//
// Created by john on 9/12/15.
//

#ifndef IANT_ARGOS_CHROMOSOME_H
#define IANT_ARGOS_CHROMOSOME_H

#include <vector>
#include <argos3/core/utility/string_utilities.h>

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

    Gene* getGene(int i) {return genes.at(i);}
    int getSize() {return genes.size();}
    void addGene(Gene* gene){genes.push_back(gene);}

private:
    vector<Gene*> genes;
};


#endif //IANT_ARGOS_CHROMOSOME_H
