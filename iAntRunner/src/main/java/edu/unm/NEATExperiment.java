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
        Neat.p_survival_thresh = 0.1;

        Neat.p_trait_param_mut_prob = 0.5;
        Neat.p_trait_mutation_power = 1.0;
        Neat.p_linktrait_mut_sig = 1.0;
        Neat.p_nodetrait_mut_sig = 0.5;
        Neat.p_weight_mut_power = 2.5;
        Neat.p_recur_prob = 0.1;
        Neat.p_disjoint_coeff = 1.0;
        Neat.p_excess_coeff = 1.0;
        Neat.p_mutdiff_coeff = 0.4;
        Neat.p_compat_threshold = 3.0;
        Neat.p_age_significance = 1.0;
        Neat.p_survival_thresh = 0.2;
        Neat.p_mutate_only_prob = 0.25;
        Neat.p_mutate_random_trait_prob = 0.1;
        Neat.p_mutate_link_trait_prob = 0.1;
        Neat.p_mutate_node_trait_prob = 0.1;
        Neat.p_mutate_link_weights_prob = 0.9;
        Neat.p_mutate_toggle_enable_prob = 0.0;
        Neat.p_mutate_gene_reenable_prob = 0.0;
        Neat.p_mutate_add_node_prob = 0.03;
        Neat.p_mutate_add_link_prob = 0.08;
        Neat.p_interspecies_mate_rate = 0.0010;
        Neat.p_mate_multipoint_prob = 0.3;
        Neat.p_mate_multipoint_avg_prob = 0.3;
        Neat.p_mate_singlepoint_prob = 0.3;
        Neat.p_mate_only_prob = 0.2;
        Neat.p_recur_only_prob = 0.0;;
        Neat.p_dropoff_age = 50;
        Neat.p_newlink_tries = 50;
        Neat.p_print_every=  10;
        Neat.p_babies_stolen = 0;
        Neat.p_num_runs = 1;
        Neat.p_num_trait_params = 8;

        Population population = new Population(Neat.p_pop_size, 16, 3, 5, true, 1);


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
