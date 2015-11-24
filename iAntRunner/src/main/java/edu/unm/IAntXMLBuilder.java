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

    public String buildXML(String chromosome, int seed, int runtime, int distribution, int entityCount) {
        return iAntXml
                .replace("${chromosome}", chromosome)
                .replace("${seed}", Integer.toString(seed))
                .replace("${distribution}", Integer.toString(distribution))
                .replace("${runtime}", Integer.toString(runtime))
                .replace("${entitycount}", Integer.toString(entityCount));
    }
}
