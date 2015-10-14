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

    string filename = "experiments/gant";

    if(argc > 1) {
        filename += argv[1];
    }

    filename += ".xml";

    ofstream outfile (filename.c_str());

    //read stdin to the iant file
    for (std::string line; std::getline(std::cin, line);) {
        outfile << line << std::endl;
    }

    static argos::CSimulator& cSimulator = argos::CSimulator::GetInstance();

    cSimulator.SetExperimentFileName(filename.c_str());
    cSimulator.LoadExperiment();

    iAnt_loop_functions &cLoopFunctions = (iAnt_loop_functions&)(cSimulator.GetLoopFunctions());

    cSimulator.Reset();
    cSimulator.Execute();

    cout << "Fitness:" << cLoopFunctions.getFitness() << endl;

    /*
     * Dispose of ARGoS stuff
     */
    cSimulator.Destroy();

    /* All is OK */
    return 0;
}

/****************************************/
/****************************************/
