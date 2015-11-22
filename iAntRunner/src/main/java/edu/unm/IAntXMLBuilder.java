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

    public String buildXML(String chromosome, String seed) {
        return iAntXml.replace("${chromosome}", chromosome)
                .replace("${seed}", seed);
    }
}
