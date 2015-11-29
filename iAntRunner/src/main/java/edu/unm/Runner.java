package edu.unm;

import java.util.ArrayList;
import java.util.List;

/**
 * @author John Ericksen
 */
public class Runner {

    public static void main(String[] args) throws Exception {

        int populationSize = 100;
        int runtime = 10 * 60;
        int localRunnerCount = Runtime.getRuntime().availableProcessors();
        int remoteRunnerCount = 0;
        int[] entityCounts = {10, 25, 60};
        int[] distributions = {0, 1, 2};

        List<OrganismExecutor> executors = new ArrayList<OrganismExecutor>();

        for(int entityCount : entityCounts) {
            for (int distribution : distributions) {
                long startTime = System.currentTimeMillis();
                Logger log = new Logger(startTime);

                //setup local runners
                for (int i = 0; i < localRunnerCount; i++) {
                    executors.add(new LocalOrganismExecutor("/home/john/dev/iAnt-ARGoS/", log, startTime));
                }

                //setup remote runners
                for (int i = 0; i < remoteRunnerCount; i++) {
                    executors.add(new RemoteOrganismExecutor("johncarl@maricopa.cs.unm.edu", "~/iAnt-ARGOS;", log, startTime));
                }

                NEATExperiment experiment = new NEATExperiment(executors, log, populationSize, runtime, distribution, entityCount);

                experiment.run();
            }
        }

    }
}
