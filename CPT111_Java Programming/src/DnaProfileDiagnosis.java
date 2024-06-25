import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;

public class DnaProfileDiagnosis {
    
    // you may modify/add more instance variables
    // but your algorithms must primarily use the following list and/or map
    private LinkedList<String> list;
    private HashMap<String,LinkedList<String>> map;
    private String dna;
    private LinkedList<String> dnaTemp;

    // build a database from database.csv
    public DnaProfileDiagnosis(String database){
        //The way to read csv data is cited from CPT111.lecture11
        File file = new File(database);
        try {

            FileReader input = new FileReader(file);
            BufferedReader reader = new BufferedReader(input);
            String line;
            map = new HashMap<>();

            while ((line = reader.readLine()) != null) {
                String[] values = line.split(",");
                list = new LinkedList<>();
                for (int i = 1; i < values.length; i++) {
                    list.add(values[i]);
                }
                map.put(values[0], list);
            }
            input.close();
        }catch (IOException ioe){
            ioe.getMessage();
        }
    }

    // store a dna sequence with no whitespace from dna.txt
    public void readDna(String dna){
        //The way to read file data is cited from CPT111.lecture11
        try {
            File file = new File(dna);
            FileReader input = new FileReader(file);
            BufferedReader reader = new BufferedReader(input);
            String line;
            this.dna = "";
            while ((line = reader.readLine()) != null) {
                line = line.replace(" ", "");
                line = line.replace("\n", "");
                line = line.replace("\t", "");
                this.dna += line;
            }
        } catch (IOException ioe) {
            ioe.getMessage();
        }
    }

    // based on the STR counts, return either a name in 
    // database, or "No match"
    // throws IllegalArgumentException if dna has not been set
    public String checkProfile(){
        if(dna == null){
            throw new IllegalArgumentException();
        } else{
            String returnName = "No match";
            dnaTemp = new LinkedList<>();

            for (String strTemp : map.get("name")) {
                int tempLength = strTemp.length();
                int firstFound = dna.indexOf(strTemp);
                int maxCount = 0;
                while (dna.indexOf(strTemp, firstFound) != -1) {
                    int nowCount = 0;
                    while (firstFound + tempLength <= dna.length()
                            && dna.substring(firstFound, firstFound + tempLength).equals(strTemp)) {
                        nowCount++;
                        firstFound += tempLength;
                    }
                if (maxCount < nowCount) {
                    maxCount = nowCount;
                }
                firstFound += 1;
                }
                dnaTemp.add(String.valueOf(maxCount));
            }

            for (String key : map.keySet()) {
                int compareCount = 0;
                for (int i = 0; i < map.get(key).size(); i++) {
                    if (map.get(key).get(i).equals(dnaTemp.get(i))) {
                        compareCount++;
                    }
                }
                if (compareCount == map.get(key).size()) {
                    returnName = key;
                    break;
                }
            }
            return returnName;
        }
    }
    // based on the CAG repeats, return either "Faulty Test",
    // "Normal", "High Risk", or "Huntington's"
    // throws IllegalArgumentException if dna has not been set
    public String diagnoseHd() {
        if(dna == null){
            throw new IllegalArgumentException();
        } else{
            String returnHdName = "Faulty Test";
            int maxHdCount = 0;
            int firstFound = dna.indexOf("CAG");
            while (dna.indexOf("CAG",firstFound) != -1){
                int nowHdCount = 0;
                while (firstFound + 3 <= dna.length()
                        && dna.substring(firstFound,firstFound + 3).equals("CAG")){
                    nowHdCount++;
                    firstFound += 3;
                }
                if(maxHdCount < nowHdCount){
                    maxHdCount = nowHdCount;
                }
                firstFound += 1;
            }
            if(maxHdCount >= 0 && maxHdCount <= 9){
                returnHdName = "Faulty Test";
            }else if (maxHdCount >= 10 && maxHdCount <= 35){
                returnHdName = "Normal";
            }else if (maxHdCount >= 36 && maxHdCount <= 39){
                returnHdName = "High Risk";
            }else if (maxHdCount >= 40 && maxHdCount <= 180){
                returnHdName =  "Huntington's";
            }else if(maxHdCount >= 181){
                returnHdName = "Faulty Test";
            }
            return returnHdName;
        }
    }
}
