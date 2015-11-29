package edu.unm;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

/**
 * @author John Ericksen
 */
public class Logger {

    private final PrintStream out;

    public Logger(ExperimentParameters parameters) throws FileNotFoundException {
        out = new PrintStream(new FileOutputStream("runlog" + parameters.populationSize() + ".txt"));
        log(parameters.toString());
    }

    public void log(String message){
        System.out.println(message);
        out.println(message);
        out.flush();
    }
}
