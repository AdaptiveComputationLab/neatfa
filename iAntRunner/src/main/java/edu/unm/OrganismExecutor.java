package edu.unm;

import edu.unm.neat.jneat.Organism;

import javax.management.Query;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.util.Queue;

/**
 * @author John Ericksen
 */
public abstract class OrganismExecutor {

    private static final int[] SEEDS = {2074558938, -1510851058, -1428910690, 2031391330, -1034048669, 1456499443, 1198128538, -641569456, 145210938, -393752301};
    private final IAntXMLBuilder builder;
    private final String homeDirectory;
    private final Logger log;
    private final long startTime;
    private final Map<String, Double> fitness_cache = new LRUCache<String, Double>(10000);

    public OrganismExecutor(String homeDirectory, Logger log, long startTime) throws IOException {
        this.startTime = startTime;
        this.builder = new IAntXMLBuilder();
        this.homeDirectory = homeDirectory;
        this.log = log;
    }

    public void listen(Queue<OrganismIdWrapper> queue, int epoch, ExperimentParameters parameters) {
        while(!queue.isEmpty()){
            final OrganismIdWrapper organismWrapper = queue.poll();
            if(organismWrapper != null) {
                final Organism organism = organismWrapper.getOrganism();
                final String chromosome = organismWrapper.buildChromosome();
                final String tag = startTime + "E" + epoch + "C" + organismWrapper.getId();

                if (fitness_cache.containsKey(chromosome)) {
                    organism.setFitness(organism.getFitness() + fitness_cache.get(chromosome));
                } else {
                    for (int i = 0; i < SEEDS.length; i++) {
                        InputStream xml = new ByteArrayInputStream(builder.buildXML(chromosome, SEEDS[i], parameters).getBytes());

                        new ProcessExecutable(homeDirectory, getExecutable(tag), xml,
                                new ProcessExecutable.OnResultCallback() {
                                    @Override
                                    public void onResult(Long result) {
                                        organism.setFitness(organism.getFitness() + result);
                                    }
                                }
                        ).run();
                    }
                    organism.setFitness(organism.getFitness() / (SEEDS.length));
                    fitness_cache.put(chromosome, organism.getFitness());
                }

                log.log("done: Fitness: " + organism.getFitness() + " Chromosome: " + tag + " " + chromosome);
            }
        }
    }

    public abstract String[] getExecutable(String tag);
}
