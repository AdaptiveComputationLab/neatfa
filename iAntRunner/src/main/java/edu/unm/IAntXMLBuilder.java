package edu.unm;

import org.apache.commons.io.IOUtils;

import java.io.FileInputStream;
import java.io.IOException;

/**
 * @author John Ericksen
 */
public class IAntXMLBuilder {

    private final String iAntXml;

    public IAntXMLBuilder() throws IOException {
        iAntXml = IOUtils.toString(new FileInputStream("iAnt.xml"));
    }

    public String buildXML(String chromosome, int seed, ExperimentParameters parameters) {
        return iAntXml
                .replace("${chromosome}", chromosome)
                .replace("${seed}", Integer.toString(seed))
                .replace("${distribution}", Integer.toString(parameters.distribution()))
                .replace("${runtime}", Integer.toString(parameters.runtime()))
                .replace("${entitycount}", Integer.toString(parameters.entityCount()));
    }
}
