package edu.unm;

import edu.unm.neat.jneat.Gene;
import edu.unm.neat.jneat.Organism;

/**
 * @author John Ericksen
 */
public class OrganismIdWrapper {

    private final Organism organism;
    private final int id;

    public OrganismIdWrapper(Organism organism, int id) {
        this.organism = organism;
        this.id = id;
    }

    public Organism getOrganism() {
        return organism;
    }

    public int getId() {
        return id;
    }

    public String buildChromosone() {
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
