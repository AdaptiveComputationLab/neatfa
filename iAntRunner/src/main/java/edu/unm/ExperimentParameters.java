package edu.unm;

import com.google.auto.value.AutoValue;

/**
 * @author John Ericksen
 */
@AutoValue
public abstract class ExperimentParameters {

    public abstract int populationSize();

    public abstract int runtime();

    public abstract int entityCount();

    public abstract int distribution();

    public abstract long startTime();

    @AutoValue.Builder
    abstract static class Builder {
        public abstract Builder populationSize(int size);

        public abstract Builder runtime(int runtime);

        public abstract Builder entityCount(int count);

        public abstract Builder distribution(int distribution);

        public abstract Builder startTime(long time);

        abstract ExperimentParameters build();
    }
}
