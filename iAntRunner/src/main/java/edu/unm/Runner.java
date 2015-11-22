package edu.unm;

import java.util.ArrayList;
import java.util.List;

/**
 * @author John Ericksen
 */
public class Runner {

    public static void main(String[] args) throws Exception {

        long startTime = System.currentTimeMillis();
        int populationSize = 100;
        int localRunnerCount = 10;
        int remoteRunnerCount = 0;

        Logger log = new Logger(startTime);
        List<OrganismExecutor> executors = new ArrayList<OrganismExecutor>();

        //setup local runners
        for (int i = 0; i < localRunnerCount; i++){
            executors.add(new LocalOrganismExecutor("/home/john/dev/iAnt-ARGoS/", log, startTime));
        }

        //setup remote runners
        for (int i = 0; i < remoteRunnerCount; i++){
            executors.add(new RemoteOrganismExecutor("johncarl@maricopa.cs.unm.edu", "~/iAnt-ARGOS;", log, startTime));
        }

        NEATExperiemnt experiment = new NEATExperiemnt(executors, log, populationSize);

        experiment.run();

    }
}
