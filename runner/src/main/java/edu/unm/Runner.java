package edu.unm;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author John Ericksen
 */
public class Runner {

    public static void main(String[] args) throws Exception {

        Options options = new Options();

        options.addOption("p", "populationSize", true, "Size of the experiment controller collection to evolve.");
        options.addOption("r", "runtime", true, "Total runtime of each run of ARGOS in seconds.");
        options.addOption("d", "distribution", true, "Seed distribution to use in the experiment, 0 = Random, 1 = Clustered, 2 = Semi-clustered.");
        options.addOption("m", "multiDistriution", false, "Run the experiment against all three distributions.");
        options.addOption("c", "entityCount", true, "Number of experimental swarm entities.");
        options.addOption("a", "argosDirectory", true, "Root directory of argos.");
        options.addOption("h", "help", false, "Prints this message.");
        options.addOption("t", "template", true, "iAnt.xml template file to use.");
        options.addOption("e", "epochs", true, "Number of epochs to run the experiment.");

        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse( options, args);

        if(cmd.hasOption("h")) {
            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp( Runner.class.getCanonicalName(), options );
        }
        else {

            int populationSize = parseIntegerOption(cmd, "p", 100);
            int runtime = parseIntegerOption(cmd, "r", 30 * 60);
            int distribution = parseIntegerOption(cmd, "r", 0);
            boolean multiDistribution = cmd.hasOption("m");
            int entityCount = parseIntegerOption(cmd, "r", 6);
            int epochs = parseIntegerOption(cmd, "e", 100);
            String argosDirectory = "argos";
            if (cmd.hasOption("a")) {
                argosDirectory = cmd.getOptionValue("a");
            }
            String templateFile = "iAnt.xml";
            if(cmd.hasOption("t")) {
                templateFile = cmd.getOptionValue("t");
            }

            List<Integer> distributions = new ArrayList<Integer>();

            if(multiDistribution) {
                distributions.addAll(Arrays.asList(0, 1, 2));
            }
            else {
                distributions.add(distribution);
            }

            int localRunnerCount = Runtime.getRuntime().availableProcessors();

            ExperimentParameters.Builder parametersBuilder = ExperimentParameters.builder();

            parametersBuilder
                    .populationSize(populationSize)
                    .runtime(runtime);

            parametersBuilder
                    .entityCount(entityCount)
                    .startTime(System.currentTimeMillis());

            ExperimentParameters parameters = parametersBuilder.build();

            Logger log = new Logger(parameters);

            List<OrganismExecutor> executors = new ArrayList<OrganismExecutor>();

            //setup local runners
            for (int i = 0; i < localRunnerCount; i++) {
                executors.add(new LocalOrganismExecutor(argosDirectory, log, parameters.startTime(), templateFile));
            }

            NEATExperiment experiment = new NEATExperiment(executors, log, parameters, epochs, distributions);

            experiment.run();
        }
    }

    private static int parseIntegerOption(CommandLine cmd, String parameterName, int defaultValue) {
        if(cmd.hasOption(parameterName)) {
            String input = cmd.getOptionValue(parameterName);
            try {
                return Integer.parseInt(input);
            } catch (NumberFormatException e) {
                System.out.println("Parameter " + parameterName + " must be an integer, using default value of " + defaultValue + ".");
            }
        }
        return defaultValue;
    }

}
