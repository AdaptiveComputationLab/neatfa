package edu.unm;

import com.google.auto.value.AutoValue;

/**
 * @author John Ericksen
 */
@AutoValue
public abstract class XMLParameters {

    public abstract String cromosome();
    public abstract int seed();
    public abstract int dist();

    public static XMLParameters build(String chromosome, int seed, int dist) {
        return new AutoValue_XMLParameters(chromosome, seed, dist);
    }
}
