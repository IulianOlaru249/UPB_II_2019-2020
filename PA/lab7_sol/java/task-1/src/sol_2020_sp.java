import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Queue;
import java.util.LinkedList;

public class sol_2020_sp {
    static class Task {
        public static final String INPUT_FILE = "in";
        public static final String OUTPUT_FILE = "out";
        public static final int NMAX = 100005; // 10^5
        public static final int Inf = 0x3f3f3f3f;

        int n;
        int m;

        @SuppressWarnings("unchecked")
        ArrayList<Integer> adj[] = new ArrayList[NMAX];

        int source;

        private void readInput() {
            try {
                Scanner sc = new Scanner(new BufferedReader(new FileReader(
                                INPUT_FILE)));
                n = sc.nextInt();
                m = sc.nextInt();
                source = sc.nextInt();

                for (int i = 1; i <= n; i++)
                    adj[i] = new ArrayList<>();
                for (int i = 1; i <= m; i++) {
                    int x, y;
                    x = sc.nextInt();
                    y = sc.nextInt();
                    adj[x].add(y);
                    adj[y].add(x);
                }
                sc.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private void writeOutput(int[] result) {
            try {
                PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(
                                OUTPUT_FILE)));
                for (int i = 1; i <= n; i++) {
                    pw.printf("%d%c", result[i], i == n ? '\n' : ' ');
                }
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private int[] getResult() {
            int d[] = new int[n + 1];
            for (int i = 0; i <= n; i++) d[i] = Inf;
            // TODO: Faceti un BFS care sa construiasca in d valorile d[i] = numarul
            // minim de muchii de parcurs de la nodul source la nodul i.
            // d[source] = 0
            // d[x] = -1 daca nu exista drum de la source la x.
            // *******
            // ATENTIE: nodurile sunt indexate de la 1 la n.
            // *******

            // initializare coada si adaugarea sursei in coada
            Queue<Integer> q = new LinkedList<>();
            q.add(source);
            d[source] = 0;

            // cat timp mai sunt noduri neprocesate in coada
            while (!q.isEmpty()) {
                // extragem si procesam urmatorul nod din coada
                int node = q.poll();

                // parcurgem vecinii nodului
                for (int v: adj[node]) {
                    // daca nodul nu a fost descoperit inca (d[v] == Inf)
                    if (d[v] == Inf) {
                        // adauga nodul in coada si actualizeaza distanta pana la el
                        q.add(v);
                        d[v] = d[node] + 1;
                    }
                }
            }

            for (int i = 1; i <= n; i++)
                if (d[i] == Inf)
                    d[i] = -1;

            return d;
        }

        public void solve() {
            readInput();
            writeOutput(getResult());
        }
    }

    public static void main(String[] args) {
        new Task().solve();
    }
}
