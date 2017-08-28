package edu.unm;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * @author John Ericksen
 */
public class FileStatisticsRunner {

    private static final Random RAND = new Random(System.currentTimeMillis());
    private static final String HOME_DIRECTORY = "/home/john/dev/iAnt-ARGoS/";
    private static final String EXECUTABLE = "./build/source/iant_main";

    //"runlog1495307324523.txt"

    private static final String[] FILES = {
            "runlog1480314453509.txt",
            "runlog1480325980460.txt",
            "runlog1480340772516.txt",
            "runlog1495307324523.txt"
    };

    private static final int[] COUNT = {1, 4, 6, 8, 10, 15, 20, 25, 30};

    public static void main(String[] args) throws Exception {

        for(int i = 0; i < FILES.length; i++) {
            String file = FILES[i];
            int count = COUNT[i/3];
            runStats(file, getFirstLine(file), 6, getMaxChromosome(file));
            //System.out.println("For: " + file + " max: " + getMaxChromosome(file));
        }
    }

    /*
    public static void main(String[] args) throws Exception {

        for(int i = 0; i < FILES.length; i++) {
            //for(int j = 0; j < COUNT.length; j++) {
                String file = FILES[i];
                //runStats(file, getFirstLine(file), COUNT[j], getMaxChromosome(file));
                //System.out.println("For: " + file + " max: " + getMaxChromosome(file));
                //System.out.println("max: " + getMaxChromosome(file));
                Set<Integer> nodes = new HashSet<Integer>();
                int deacivatedNodes = 0;
                int edges = 0;

                String maxChromosome = getMaxChromosome(file);
                for(String chromosome : maxChromosome.substring(maxChromosome.indexOf(";") + 1).split(";")) {
                    String[] parts = chromosome.split(",");

                    if(parts[1].equals("1")) {
                        nodes.add(Integer.parseInt(parts[2]));
                        nodes.add(Integer.parseInt(parts[3]));
                        edges++;
                    }
                    else {
                        deacivatedNodes++;
                    }
                }
                System.out.println("Nodes: " + nodes.size());
                System.out.println("Acive edges: " + edges);
            System.out.println("deacivatedNodes edges: " + deacivatedNodes);
            //}
        }
    }
     */

    private static void runStats(String file, String line, int entityCount, String chromosome) throws Exception {
        System.out.println(chromosome);
        IAntXMLBuilder builder = new IAntXMLBuilder("iAnt.xml");

        AutoValue_ExperimentParameters.Builder parametersBuilder = new AutoValue_ExperimentParameters.Builder();

        parametersBuilder
                .populationSize(100)
                .runtime(30 * 60)
                .entityCount(entityCount);

        long startTime = System.currentTimeMillis();
        for(int d = 0; d < 3; d++) {


            parametersBuilder
                    .distribution(d)
                    .startTime(System.currentTimeMillis());

            System.out.println(parametersBuilder.build());


            final List<Long> fitness = new ArrayList<Long>();
            ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());

            for(int j = 0; j < 48; j++) {

                InputStream xml = new ByteArrayInputStream(builder.buildXML(chromosome, RAND.nextInt(65536), parametersBuilder.build()).getBytes());

                final String tag = startTime + "F" + file + "D" + d + "C" + j;

                executor.submit(
                        new ProcessExecutable(HOME_DIRECTORY, new String[]{EXECUTABLE, tag}, xml,
                                new ProcessExecutable.OnResultCallback() {
                                    @Override
                                    public void onResult(Long result) {
                                        //System.out.println("Tag: " + tag + " Fitness: " + result);
                                        fitness.add(result);
                                    }
                                }
                        ));
            }

            executor.shutdown();

            while (!executor.awaitTermination(1, TimeUnit.SECONDS)) {}
            //System.out.println("For: " + file + " : " + line + " Count: " + entityCount + " Dist: " + d + " Stats: " + getStatistics(fitness));
            System.out.println("Entities: " + entityCount + " Dist: " + d + " : " + getStatistics(fitness));
        }
    }

    private static String getStatistics(List<Long> fitness) {
        long sum = 0;

        for(Long f : fitness) {
            sum += f;
        }

        double average = (sum * 1.0) / fitness.size();

        double stddevsum = 0;
        for(Long f : fitness) {
            stddevsum += Math.pow((f - average), 2);
        }

        double stddev = Math.sqrt(stddevsum / fitness.size());

        return "Average: " + average + " Std Dev: " + stddev;
    }

    private static String getMaxChromosome(String file) throws IOException {
        File logFile = new File(new File("/home/john/neatfa/data"), file);

        BufferedReader reader = new BufferedReader(new FileReader(logFile));
        List<FitnessChromosome> chromosomes = new ArrayList<FitnessChromosome>();

        // Find max
        double max = Double.NEGATIVE_INFINITY;
        String maxChromosome = "none";
        for(String line; (line = reader.readLine()) != null; ) {
            if(line.startsWith("done: Fitness: ")) {
                /*double fitness = Double.parseDouble(line.split(" ")[2]);
                if(fitness > max) {
                    max = fitness;
                    maxChromosome = line.split(" ")[5];
                }*/
                FitnessChromosome fc = new FitnessChromosome();
                fc.fitness = Double.parseDouble(line.split(" ")[2]);
                fc.chromosome = line.split(" ")[5];
                chromosomes.add(fc);
            }
        }

        Collections.sort(chromosomes, new Comparator<FitnessChromosome>() {
            @Override
            public int compare(FitnessChromosome o1, FitnessChromosome o2) {
                return Double.compare(o2.fitness, o1.fitness);
            }
        });

        return chromosomes.get(0).chromosome;
    }

    private static class FitnessChromosome {
        double fitness;
        String chromosome;
    }

    private static String getFirstLine(String file) throws IOException {
        File logFile = new File(new File("/home/john/neatfa/data"), file);

        BufferedReader reader = new BufferedReader(new FileReader(logFile));
        return reader.readLine();
    }
}
