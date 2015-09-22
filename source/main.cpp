//
// Created by john on 9/12/15.
//

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/loop_functions.h>
#include <source/ga/BasicGA.h>
#include <source/ga/ChromosomeFactory.h>
#include "iAnt_controller.h"
#include "iAnt_loop_functions.h"
#include "iAnt_pheromone.h"

/****************************************/
/****************************************/

int main(int argc, char** argv) {

    static argos::CSimulator& cSimulator = argos::CSimulator::GetInstance();

    cSimulator.SetExperimentFileName("experiments/iAnt.xml");
    cSimulator.LoadExperiment();

   static iAnt_loop_functions &cLoopFunctions = static_cast<iAnt_loop_functions&>(cSimulator.GetLoopFunctions());
    BasicGA ga;
    ChromosomeFactory chromosomeFactory;
    vector<Chromosome*> chromosomes = chromosomeFactory.buildPopulation(10);

    for(int i = 0; i < 100; i++) {
        argos::LOG << "Generation #";
        argos::LOG << i;
        argos::LOG << " started.\n";
        vector<BasicGA::FitnessChromosome> fitness;

        int fitsum = 0;

        for (int c = 0; c < chromosomes.size(); c++) {
            cLoopFunctions.setChromosome(chromosomes.at(c));
	        cSimulator.Reset();
            cSimulator.Execute();


            BasicGA::FitnessChromosome fitChromosome;
            fitChromosome.chromosome = chromosomes.at(c);
            fitChromosome.fitness = cLoopFunctions.getFitness();
            fitsum += fitChromosome.fitness;
            fitness.push_back(fitChromosome);
        }

        argos::LOG << "Generation #";
        argos::LOG << i;
        argos::LOG << " total: ";
        argos::LOG << fitsum;
        argos::LOG <<"done.\n";

        chromosomes = ga.evolve(fitness);
    }

    /*
     * Dispose of ARGoS stuff
     */
    cSimulator.Destroy();

    /* All is OK */
    return 0;
}

/****************************************/
/****************************************/
