//
// Created by john on 9/12/15.
//

#include <argos3/core/simulator/simulator.h>

/****************************************/
/****************************************/

int main(int argc, char** argv) {

    argos::CSimulator& cSimulator = argos::CSimulator::GetInstance();
    cSimulator.SetExperimentFileName("experiments/iAnt.argos");
    cSimulator.LoadExperiment();

    argos::LOG << "Generation #";
    argos::LOG << "done.";

    cSimulator.Execute();

    /*
     * Dispose of ARGoS stuff
     */
    cSimulator.Destroy();

    /* All is OK */
    return 0;
}

/****************************************/
/****************************************/