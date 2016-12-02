package edu.unm;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * @author John Ericksen
 */
public class FileStatisticsRunner {

    private static final Random RAND = new Random();
    private static final String HOME_DIRECTORY = "/home/john/dev/iAnt-ARGoS/";
    private static final String EXECUTABLE = "./build/source/iant_main";

    private static final String[] FILES = {
            "runlog1480280318419.txt",
            "runlog1480283190725.txt",
            "runlog1480286149378.txt",
            "runlog1480288601422.txt",
            "runlog1480297240616.txt",
            "runlog1480305792928.txt",
            "runlog1480314453509.txt",
            "runlog1480325980460.txt",
            "runlog1480340772516.txt",
            "runlog1480352697854.txt",
            "runlog1480365735799.txt",
            "runlog1480379220281.txt",
            "runlog1480396051049.txt",
            "runlog1480421973717.txt",
            "runlog1480445126285.txt",
            "runlog1480462416902.txt",
            "runlog1480492180722.txt",
            "runlog1480517527448.txt"
    };

    private static final int[] COUNT = {1, 4, 6, 8, 10, 15, 20, 25, 30};

    public static void main(String[] args) throws Exception {

        for(int i = 0; i < FILES.length; i++) {
            String file = FILES[i];
            int count = COUNT[i/3];
            //runStats(file, getFirstLine(file), count, getMaxChromosome(file));
            System.out.println("For: " + file + " max: " + getMaxChromosome(file));
        }
    }

    private static void runStats(String file, String line, int entityCount, String chromosome) throws Exception {
        IAntXMLBuilder builder = new IAntXMLBuilder();

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
            System.out.println("For: " + file + " : " + line + " Count: " + entityCount + " Dist: " + d + " Stats: " + getStatistics(fitness));

        }
    }

    private static String getStatistics(List<Long> fitness) {
        long sum = 0;

        for(Long f : fitness) {
            sum += f;
        }

        double average = sum / fitness.size();

        double stddevsum = 0;
        for(Long f : fitness) {
            stddevsum += Math.pow((f - average), 2);
        }

        double stddev = Math.sqrt(stddevsum / fitness.size());

        return "Average: " + average + " Std Dev: " + stddev;
    }

    private static String getMaxChromosome(String file) throws IOException {
        File logFile = new File(new File("/home/john/experiemnt_data"), file);

        BufferedReader reader = new BufferedReader(new FileReader(logFile));

        // Find max
        double max = Double.NEGATIVE_INFINITY;
        String maxChromosome = "none";
        for(String line; (line = reader.readLine()) != null; ) {
            if(line.startsWith("done: Fitness: ")) {
                double fitness = Double.parseDouble(line.split(" ")[2]);
                if(fitness > max) {
                    max = fitness;
                    maxChromosome = line.split(" ")[4];
                }
            }
        }
        return maxChromosome;
    }

    private static String getFirstLine(String file) throws IOException {
        File logFile = new File(new File("/home/john/experiemnt_data"), file);

        BufferedReader reader = new BufferedReader(new FileReader(logFile));
        return reader.readLine();
    }
}
