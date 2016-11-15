package edu.unm;

import java.util.ArrayList;
import java.util.List;

/**
 * @author John Ericksen
 */
public class Runner {

    public static void main(String[] args) throws Exception {

        int localRunnerCount = Runtime.getRuntime().availableProcessors();
        int remoteRunnerCount = 0;

        AutoValue_ExperimentParameters.Builder parametersBuilder = new AutoValue_ExperimentParameters.Builder();

        parametersBuilder
                .populationSize(100)
                .runtime(30 * 60);

        int[] entityCounts = {1, 4, 6, 8, 10, 15, 20, 25, 30};
        int[] distributions = {0, 1, 2};

        for(int entityCount : entityCounts) {
            for (int distribution : distributions) {

                parametersBuilder
                        .entityCount(entityCount)
                        .distribution(distribution)
                        .startTime(System.currentTimeMillis());

                ExperimentParameters parameters = parametersBuilder.build();

                Logger log = new Logger(parameters);

                List<OrganismExecutor> executors = new ArrayList<OrganismExecutor>();

                //setup local runners
                for (int i = 0; i < localRunnerCount; i++) {
                    executors.add(new LocalOrganismExecutor("/nfs/adaptive/johncarl/iant/", log, parameters.startTime()));
                }

                //setup remote runners
                for (int i = 0; i < remoteRunnerCount; i++) {
                    executors.add(new RemoteOrganismExecutor("johncarl@maricopa.cs.unm.edu", "~/iAnt-ARGOS;", log, parameters.startTime()));
                }

                NEATExperiment experiment = new NEATExperiment(executors, log, parameters);

                experiment.run();
            }
        }

    }
}
