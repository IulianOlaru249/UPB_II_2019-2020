import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Queue;
import java.util.LinkedList;

public class sol_2020_sp2 {
    static class Task {
        public static final String INPUT_FILE = "in";
        public static final String OUTPUT_FILE = "out";
        public static final int NMAX = 100005; // 10^5

        int n;
        int m;

        @SuppressWarnings("unchecked")
        ArrayList<Integer> adj[] = new ArrayList[NMAX];
        int[] in_degree;

        private void readInput() {
            try {
                Scanner sc = new Scanner(new BufferedReader(new FileReader(
                                INPUT_FILE)));
                n = sc.nextInt();
                m = sc.nextInt();

                // in_degree va contoriza gradul intern al nodurilor (cate arce intra intr-un nod).
                in_degree = new int[n + 1];

                for (int i = 1; i <= n; i++)
                    adj[i] = new ArrayList<>();
                for (int i = 1; i <= m; i++) {
                    int x, y;
                    x = sc.nextInt();
                    y = sc.nextInt();
                    adj[x].add(y);
                    ++in_degree[y];
                }
                sc.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private void writeOutput(ArrayList<Integer> result) {
            try {
                PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(
                                OUTPUT_FILE)));
                for (int i = 0; i < result.size(); i++) {
                    pw.printf("%d ", result.get(i));
                }
                pw.printf("\n");
                pw.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }

        private ArrayList<Integer> getResult() {
            // TODO: Faceti sortarea topologica a grafului stocat cu liste de
            // adiacenta in adj.
            // *******
            // ATENTIE: nodurile sunt indexate de la 1 la n.
            // *******
            return solve_bfs(); // BFS: O(n + m)
        }

        private ArrayList<Integer> solve_bfs() {
            // Declaram o coada ce va contine toate nodurile care nu mai au dependente
            // si pe care le putem adauga apoi in topsort.
            Queue<Integer> q = new LinkedList<>();
            ArrayList<Integer> topsort = new ArrayList<>();

            // Initial adaugam in coada toate nodurile cu grad intern 0 (fara dependente)
            for (int i = 1; i <= n; i++)
                if (in_degree[i] == 0)
                    q.add(i);

            // Cat timp mai sunt noduri in coada
            while (!q.isEmpty()) {
                // Scot primul nod din coada si il adaug la solutie
                int node = q.poll();
                topsort.add(node);

                // Parcurg toti vecinii nodului si sterg arcele catre acestia
                for (int v: adj[node]) {
                    // Obs: nu e nevoie sa sterg efectiv arcul, ci pot simula asta prin scaderea gradului intern
                    --in_degree[v];

                    // daca gradul intern al lui v a ajuns la 0 (nu mai are dependente), il adaug in coada
                    if (in_degree[v] == 0)
                        q.add(v);
                }
            }

            // Daca topsort nu contine toate nodurile, atunci graful este ciclic, deci nu are o sortare topologica
            if (topsort.size() < n)
                return new ArrayList<>();
            return topsort;
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
