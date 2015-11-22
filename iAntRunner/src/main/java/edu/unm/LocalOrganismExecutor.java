package edu.unm;

import java.io.IOException;

/**
 * @author John Ericksen
 */
public class LocalOrganismExecutor extends OrganismExecutor {

    private static final String EXECUTABLE = "./build/source/iant_main";

    public LocalOrganismExecutor(String argosHomeDirectory, Logger log, long startTime) throws IOException {
        super(argosHomeDirectory, log, startTime);
    }

    @Override
    public String[] getExecutable(String tag) {
        return new String[]{EXECUTABLE, tag};
    }
}
