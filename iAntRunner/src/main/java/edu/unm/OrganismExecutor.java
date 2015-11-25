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

    public void listen(Queue<OrganismIdWrapper> queue, int epoch, int runtime, int distribution, int entityCount) {
        while(!queue.isEmpty()){
            final OrganismIdWrapper organismWrapper = queue.poll();
            if(organismWrapper != null){
                final Organism organism = organismWrapper.getOrganism();
                final String chromosome = organismWrapper.buildChromosone();
                InputStream xml = new ByteArrayInputStream(builder.buildXML(chromosome, RAND.nextInt(65536), runtime, distribution, entityCount).getBytes());

                final String tag = startTime + "E" + epoch + "C" + organismWrapper.getId();

                new ProcessExecutable(homeDirectory, getExecutable(tag), xml,
                        new ProcessExecutable.OnResultCallback() {
                            @Override
                            public void onResult(Long result) {
                                organism.setFitness(result);
                                log.log("done: Fitness: " + result + " Chromosone: " + tag + " " + chromosome);
                            }
                        }
                ).run();
            }
        }
    }

    public abstract String[] getExecutable(String tag);
}
