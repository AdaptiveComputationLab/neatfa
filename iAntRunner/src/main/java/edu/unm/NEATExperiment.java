package edu.unm;

import edu.unm.neat.jneat.Neat;
import edu.unm.neat.jneat.Organism;
import edu.unm.neat.jneat.Population;

import java.util.List;
import java.util.concurrent.*;

/**
 * @author John Ericksen
 */
public class NEATExperiment {

    private final Logger log;
    private final List<OrganismExecutor> executors;
    private final int populationSize;
    private final int runtime;
    private final int distribution;
    private final int entityCount;

    public NEATExperiment(List<OrganismExecutor> executors, Logger log, int populationSize, int runtime, int distribution, int entityCount) {
        this.executors = executors;
        this.log = log;
        this.populationSize = populationSize;
        this.runtime = runtime;
        this.distribution = distribution;
        this.entityCount = entityCount;
    }

    public void run() throws Exception {
        long start = System.currentTimeMillis();

        Neat.initbase();

        Neat.p_pop_size = populationSize;

        Population population = new Population(Neat.p_pop_size, 11, 2, 5, true, 1);
        population.verify();


        for (int e = 0; e < 100; e++) {

            long epochStart = System.currentTimeMillis();
            log.log("Epoch " + e);

            final BlockingQueue<OrganismIdWrapper> queue = new ArrayBlockingQueue<OrganismIdWrapper>(populationSize);

            for (int i = 0; i < population.getOrganisms().size(); i++) {
                queue.add(new OrganismIdWrapper((Organism) population.organisms.get(i), i));
            }

            ExecutorService executor = Executors.newFixedThreadPool(executors.size());

            final int epoch = e;
            for (final OrganismExecutor organismExecutor : executors) {
                executor.execute(new Runnable() {
                    @Override
                    public void run() {
                        organismExecutor.listen(queue, epoch, runtime, distribution, entityCount);
                    }
                });
            }

            executor.shutdown();

            while (!executor.awaitTermination(1, TimeUnit.SECONDS)) {}

            outputStatistics(e, population);

            population.epoch(e);

            log.log("Epoch " + e + " took " + (System.currentTimeMillis() - epochStart) + "ms");
        }

        log.log("Finished: " + (System.currentTimeMillis() - start) + "ms");
    }

    private void outputStatistics(int epoch, Population population) {
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        double sum = 0;

        for (int i = 0; i < population.getOrganisms().size(); i++) {
            final Organism organism = (Organism) population.organisms.get(i);

            organism.getFitness();

            sum += organism.getFitness();
            min = Math.min(min, organism.getFitness());
            max = Math.max(max, organism.getFitness());
        }

        double average = sum / population.getOrganisms().size();

        log.log("Statistics Epoch " + epoch + ": " + sum + ", " + min + ", " + max + ", " + average);
    }
}
