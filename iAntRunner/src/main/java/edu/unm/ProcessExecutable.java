package edu.unm;

import org.apache.commons.io.IOUtils;

import java.io.*;

/**
 * @author John Ericksen
 */
public class ProcessExecutable implements Runnable {

    private final String directory;
    private final String[] executable;
    private final InputStream pipeIn;
    private final OnResultCallback callback;
    private Long result;

    public ProcessExecutable(String directory, String[] executable, InputStream pipeIn, OnResultCallback callback) {
        this.directory = directory;
        this.pipeIn = pipeIn;
        this.executable = executable;
        this.callback = callback;
    }

    @Override
    public void run() {
        try {
            ProcessBuilder processBuilder = new ProcessBuilder(executable);
            processBuilder.directory(new File(directory));
            Process process = processBuilder.start();

            OutputStream stdIn = process.getOutputStream();
            InputStream stdOut = process.getInputStream();

            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(stdIn));
            IOUtils.copy(pipeIn, stdIn);

            writer.newLine();
            writer.flush();
            writer.close();

            int code = process.waitFor();

            result = processOutput(stdOut);

            callback.onResult(result);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private Long processOutput(InputStream stdOut) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(stdOut));

        String line;
        while ((line = reader.readLine()) != null) {
            if (line.startsWith("Fitness:")) {
                String fitnessString = line.substring("Fitness:".length()).trim();
                return Long.parseLong(fitnessString);
            }
        }
        return null;
    }

    public Long getResult() {
        return result;
    }

    public interface OnResultCallback {
        void onResult(Long result);
    }
}
