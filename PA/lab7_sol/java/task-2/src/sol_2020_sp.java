import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Collections;

public class sol_2020_sp {
    static class Task {
        public static final String INPUT_FILE = "in";
        public static final String OUTPUT_FILE = "out";
        public static final int NMAX = 100005; // 10^5

        int n;
        int m;

        @SuppressWarnings("unchecked")
        ArrayList<Integer> adj[] = new ArrayList[NMAX];

        private void readInput() {
            try {
                Scanner sc = new Scanner(new BufferedReader(new FileReader(
                                INPUT_FILE)));
                n = sc.nextInt();
                m = sc.nextInt();

                for (int i = 1; i <= n; i++)
                    adj[i] = new ArrayList<>();
                for (int i = 1; i <= m; i++) {
                    int x, y;
                    x = sc.nextInt();
                    y = sc.nextInt();
                    adj[x].add(y);
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

            return solveDfs(); // DFS: O(n + m)
        }

        private ArrayList<Integer> solveDfs() {
            // vectorul rezultat (in final contine o permutare pentru 1, 2, ... n)
            ArrayList<Integer> topsort = new ArrayList<>();

            boolean[] visited = new boolean[n + 1];

            // pentru fiecare nod
            for (int i = 1; i <= n; i++)
                // daca nodul nu a fost vizitat, pornim o parcurgere DFS
                if (!visited[i])
                    dfs(i, visited, topsort);

            // rezultatul a fost obtinut in ordine inversa
            Collections.reverse(topsort);

            return topsort;
        }

        // porneste o parcurgere DFS din node
        // foloseste vectorul visited pentru a marca nodurile vizitate
        void dfs(int node, boolean[] visited, ArrayList<Integer> topsort) {
            visited[node] = true;

            for (int v: adj[node])
                if (!visited[v])
                    dfs(v, visited, topsort);

            // dupa ce am terminat de vizitat nodul, adaugam nodul in sortarea topologica
            topsort.add(node);
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
