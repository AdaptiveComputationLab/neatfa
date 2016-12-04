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
        final int populationSize = 100;
        final long startTime = System.currentTimeMillis();
        final int runtime = 30 * 60;

        AutoValue_ExperimentParameters.Builder parametersBuilder = new AutoValue_ExperimentParameters.Builder();

        parametersBuilder
                .populationSize(populationSize)
                .runtime(runtime);

        int[] entityCounts = {6};
        int[] distributions = {0, 1, 2};

        for(final int entityCount : entityCounts) {
            //for (final int distribution : distributions) {

                parametersBuilder
                        .entityCount(entityCount)
                        .distribution(0)
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

                NEATExperiment experiment = new NEATExperiment(executors, log,
                        new ExperimentParamterFactory() {
                            @Override
                            public ExperimentParameters build(int epoch) {
                                return new AutoValue_ExperimentParameters.Builder()
                                        .entityCount(entityCount)
                                        .startTime(startTime)
                                        .distribution((epoch / 300) % 3)
                                        .populationSize(populationSize)
                                        .runtime(runtime)
                                        .build();
                            }
                        }
                        , populationSize);

                experiment.run();
            //}
        }

    }
}
