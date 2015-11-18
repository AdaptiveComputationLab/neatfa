package edu.unm;

import edu.unm.neat.jneat.Gene;
import edu.unm.neat.jneat.Neat;
import edu.unm.neat.jneat.Organism;
import edu.unm.neat.jneat.Population;
import org.apache.commons.io.IOUtils;

import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * @author John Ericksen
 */
public class Runner {

    private static final String DIRECTORY = "/home/john/dev/iAnt-ARGoS/";
    private static final String EXECUTABLE = "./build/source/iant_main";

    public static void main(String[] args) throws Exception {
        long start = System.currentTimeMillis();

        Neat.initbase();

        Neat.p_pop_size = 4 * 12;

        int availableProcessors = Runtime.getRuntime().availableProcessors();
        System.out.println("Available Processors: " + availableProcessors);

        String iAntXml = IOUtils.toString(new FileInputStream("iAnt.xml"));

        Population population = new Population(Neat.p_pop_size, 11, 2, 5, true, 1);
        population.verify();

        for (int e = 0; e < 100; e++) {
            System.out.println("Epoch " + e);

            long epochStart = System.currentTimeMillis();

            ExecutorService executor = Executors.newFixedThreadPool(availableProcessors);

            for (int i = 0; i < population.organisms.size(); i++) {
                final Organism organism = (Organism) population.organisms.get(i);
                final String chromosome = buildChromosone(organism);
                ProcessExecutable processExecutable = new ProcessExecutable(DIRECTORY, new String[]{EXECUTABLE, "K" + Integer.toString(i)}, new ByteArrayInputStream(iAntXml.replace("${chromosome}", chromosome).getBytes()),
                        new ProcessExecutable.OnResultCallback() {
                            @Override
                            public void onResult(Long result) {
                                organism.setFitness(result);
                                System.out.println("done: Fitness: " + result + " Chromosone: " + chromosome);
                            }
                        }
                );
                executor.execute(processExecutable);
            }

            executor.shutdown();

            while (!executor.awaitTermination(10, TimeUnit.SECONDS)) {}

            population.viewtext();
            population.epoch(e);

            System.out.println("Epoch " + e + " took " + (System.currentTimeMillis() - epochStart) + "ms");
        }

        System.out.println("Finished: " + (System.currentTimeMillis() - start) + "ms");
    }

    private static String buildChromosone(Organism organism) {
        StringBuilder builder = new StringBuilder();

        for (int i = 0; i < organism.getGenome().getGenes().size(); i++) {
            Gene gene = (Gene) organism.getGenome().getGenes().get(i);

            builder.append(gene.getInnovation_num())
                    .append(",")
                    .append(gene.getEnable() ? "1" : "0")
                    .append(",")
                    .append(gene.getLnk().getIn_node().getNode_id())
                    .append(",")
                    .append(gene.getLnk().getOut_node().getNode_id())
                    .append(",")
                    .append(gene.getLnk().getWeight())
                    .append(";");
        }

        return builder.toString();
    }
}
