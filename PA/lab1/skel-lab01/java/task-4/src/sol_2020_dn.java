import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class sol_2020_dn {
    static class Task {
        public final static String INPUT_FILE = "in";
        public final static String OUTPUT_FILE = "out";

        int base;
        int exponent;
        int mod;

        private void readInput() {
            try {
                Scanner sc = new Scanner(new File(INPUT_FILE));
                base = sc.nextInt();
                exponent = sc.nextInt();
                mod = sc.nextInt();
                sc.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private void writeOutput(long result) {
            try {
                PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
                pw.printf("%d\n", result);
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private long fastPow(long base, int exponent, int mod) {
            // TODO: Calculati (base^exponent) modulo mod in O(log exponent).

            // base^0 = 1
            if (exponent == 0)  {
                return 1;
            }

            long aux = 1;
            while (exponent != 1) {
                if (exponent % 2 == 0) {   // par
                    base = (1L * base * base) % (1L * mod);
                    exponent /= 2;
                } else {                   // impar
                    aux = (1L * aux * base) % (1L * mod);
                    exponent--;
                }
            }

            return (1L * aux * base) % (1L * mod);
        }

        public void solve() {
            readInput();
            long result = fastPow(base, exponent, mod);
            writeOutput(result);
        }
    }

    public static void main(String[] args) {
        new Task().solve();
    }
}
