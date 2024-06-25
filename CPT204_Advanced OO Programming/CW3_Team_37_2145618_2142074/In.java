import java.io.*;

/**
 * In class provides functionality to read data from files.
 * It uses BufferedReader to read text from an input stream.
 */
class In {
    private BufferedReader br;  // BufferedReader object to read text from an input stream

    /**
     * Constructor for the In class.
     * Initializes the BufferedReader to read from the specified file.
     *
     * @param fileName The name of the file to read from
     */
    public In(String fileName) {
        try {
            InputStream is = new FileInputStream(fileName);
            InputStreamReader isr = new InputStreamReader(is);
            br = new BufferedReader(isr);
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
    }

    /**
     * Reads a line of text from the input stream.
     * The line read does not include the newline character.
     *
     * @return The line read as a string, or null if an error occurs or the end of the stream is reached
     */
    public String readLine() {
        String s = null;
        try {
            s = br.readLine();
        } catch(IOException ioe) {
            ioe.printStackTrace();
        }
        return s;
    }
}
