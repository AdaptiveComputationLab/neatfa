package edu.unm;

import java.io.IOException;

/**
 * @author John Ericksen
 */
public class RemoteOrganismExecutor extends OrganismExecutor {

    private final String sshRemote;
    private final String remoteHome;

    public RemoteOrganismExecutor(String sshRemote, String remoteHome, Logger log, long startTime) throws IOException {
        super("/", log, startTime);
        this.sshRemote = sshRemote;
        this.remoteHome = remoteHome;
    }

    public String[] getExecutable(String tag) {
        return new String[]{"ssh", sshRemote, "cd", remoteHome, "nice", "-n", "19", "build/source/iant_main", tag};
    }
}
