package edu.unm;

import org.apache.commons.cli.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class MaxChromosome {

    public static void main(String[] args) throws Exception {

        Options options = new Options();

        options.addOption("f", "file", true, "NeatFA evolution log file.");
        options.addOption("h", "help", false, "Prints this message.");

        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse( options, args);

        if(cmd.hasOption("h")) {
            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp( Runner.class.getCanonicalName(), options );
        }
        else {
            String evolutionFile = cmd.getOptionValue("f");

            new MaxChromosome().run(evolutionFile);
        }
    }

    private static class FitnessChromosome implements Comparable<FitnessChromosome> {
        double fitness;
        String chromosome;

        @Override
        public int compareTo(FitnessChromosome that) {
            return Double.compare(this.fitness, that.fitness);
        }
    }

    private void run(String evolutionFile) throws IOException {
        File logFile = new File(evolutionFile);

        BufferedReader reader = new BufferedReader(new FileReader(logFile));
        List<FitnessChromosome> chromosomes = new ArrayList<FitnessChromosome>();

        // Find max
        for(String line; (line = reader.readLine()) != null; ) {
            if(line.startsWith("done: Fitness: ")) {
                FitnessChromosome fc = new FitnessChromosome();
                fc.fitness = Double.parseDouble(line.split(" ")[2]);
                fc.chromosome = line.split(" ")[5];
                chromosomes.add(fc);
            }
        }

        Collections.sort(chromosomes);

        if(chromosomes.isEmpty()) {
            System.out.println("No chromosomes found.");
        }
        else {
            System.out.println(chromosomes.get(0).chromosome);
        }
    }
}
