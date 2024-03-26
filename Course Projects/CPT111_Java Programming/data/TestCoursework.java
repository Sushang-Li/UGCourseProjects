public class TestCoursework {

    public static void main(String[] args){


        String db1 = "cw3/data/db1.csv";
        DnaProfileDiagnosis test = new DnaProfileDiagnosis(db1);
		
        String dna1 = "cw3/data/dna1.txt";
        test.readDna(dna1);
        System.out.println(test.checkProfile()); // Alice
        System.out.println(test.diagnoseHd());   // Normal

        String dna2 = "cw3/data/dna2.txt";
        test.readDna(dna2);
        System.out.println(test.checkProfile()); // Bob
        System.out.println(test.diagnoseHd());   // Huntington's
        
//        String db2 = "cw3/data/db2.csv";
//        DnaProfileDiagnosis test2 = new DnaProfileDiagnosis(db2);
//        System.out.println(test2.checkProfile()); // IllegalArgumentException thrown

        try{
            String db2 = "cw3/data/db2.csv";
            DnaProfileDiagnosis test2 = new DnaProfileDiagnosis(db2);
            System.out.println(test2.checkProfile()); // IllegalArgumentException thrown
        }catch (IllegalArgumentException ill){
            System.out.println(ill.getMessage());
        }

        DnaProfileDiagnosis test2 = new DnaProfileDiagnosis("cw3/data/db2.csv");

        test2.readDna("cw3/data/dna3.txt");
        System.out.println(test2.checkProfile());
        String db3 = "db2.csv";
        DnaProfileDiagnosis test3 = new DnaProfileDiagnosis(db3);
        try {
            System.out.println(test3.diagnoseHd());
        }
        catch (IllegalArgumentException iae) {
            System.out.println("Illegal argument exception detected!");
        }
    }
}
