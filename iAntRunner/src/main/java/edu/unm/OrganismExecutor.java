package edu.unm;

import edu.unm.neat.jneat.Organism;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Queue;
import java.util.Random;

/**
 * @author John Ericksen
 */
public abstract class OrganismExecutor {

    private static final int TRIALS = 10;
    private static final Random RAND = new Random(System.currentTimeMillis());
    private final IAntXMLBuilder builder;
    private final String homeDirectory;
    private final Logger log;
    private final long startTime;

    public OrganismExecutor(String homeDirectory, Logger log, long startTime) throws IOException {
        this.startTime = startTime;
        this.builder = new IAntXMLBuilder();
        this.homeDirectory = homeDirectory;
        this.log = log;
    }

    public void listen(Queue<OrganismIdWrapper> queue, int epoch, ExperimentParameters parameters) {
        while(!queue.isEmpty()){
            final OrganismIdWrapper organismWrapper = queue.poll();
            if(organismWrapper != null){
                final Organism organism = organismWrapper.getOrganism();
                final String chromosome = organismWrapper.buildChromosome();
                final String tag = startTime + "E" + epoch + "C" + organismWrapper.getId();

                for(int i = 0; i < TRIALS; i++) {
                    InputStream xml = new ByteArrayInputStream(builder.buildXML(chromosome, RAND.nextInt(65536), parameters).getBytes());

                    new ProcessExecutable(homeDirectory, getExecutable(tag), xml,
                            new ProcessExecutable.OnResultCallback() {
                                @Override
                                public void onResult(Long result) {
                                    organism.setFitness(organism.getFitness() + result);
                                }
                            }
                    ).run();
                }
                organism.setFitness(organism.getFitness() / TRIALS);
                log.log("done: Fitness: " + organism.getFitness() + " Chromosome: " + tag + " " + chromosome);
            }
        }
    }

    public abstract String[] getExecutable(String tag);
}
