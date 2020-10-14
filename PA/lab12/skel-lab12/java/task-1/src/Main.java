import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;

/**
 * Generic class to represent a regular pair (C++ style).
 * 
 * Use it for storing the indexes of a grid cell like this <row, col>.
 */
class Pair<T, U> {
    public T first;
    public U second;

    public Pair(T first, U second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public boolean equals(Object other) {
        Pair<T, U> p = (Pair<T, U>) other;
        
        return this.first.equals(p.first) && this.second.equals(p.second);
    }

    @Override
    public int hashCode() {
        int result = this.first == null ? 0 : this.first.hashCode();
        result = result * 31 + (this.second == null ? 0 : this.second.hashCode());
        return result;  
    }

    @Override
    public String toString() {
        return "<" + this.first + ", " + this.second + ">";
    }
}

public class Main {
    /**
     * Generic method to print the path to a goal state.
     *
     * Each element in the path is a pair containing a matrix cell's row and column.
     *
     * @param path        The path to a goal state.
     */
    public static <T, U> void printPath(List<Pair<T, U>> path) {
        System.out.println(path.size() - 1);
        for (int i = path.size() - 1; i >= 0; --i) {
            System.out.println(path.get(i).first + " " + path.get(i).second);
        }
    }

    /**
     * Method to implement the A* algorithm.
     *
     * @param M             Encoding of the grid.
     * @param rowPacman     Pacman's starting row.
     * @param colPacman     Pacman's starting column.
     * @param rowFood       Food row.
     * @param colFood       Fool column.
     *
     * @return              The shortest path between Pacman and food, determined with A*.
     *                      If no such path exists, returns an empty path.
     */
    public static List<Pair<Integer, Integer>> astar(List<List<Character>> M,
                                                     int rowPacman, int colPacman,
                                                     int rowFood, int colFood) {
        List<Pair<Integer, Integer>> path = new ArrayList<>();

        // TODO: Implement A*.

        return path;
    }

    public static void main(String[] args) {
        List<List<Character>> M = new ArrayList<>();
        List<Pair<Integer, Integer>> path;
        int rowPacman, colPacman;
        int rowFood, colFood;
        int numRows, numCols;
        char c;

        Scanner s = new Scanner(System.in);

        rowPacman = s.nextInt();
        colPacman = s.nextInt();
        rowFood = s.nextInt();
        colFood = s.nextInt();
        numRows = s.nextInt();
        numCols = s.nextInt();

        M = new ArrayList<>();

        for (int i = 0; i < numRows; ++i) {
            List<Character> currentRow = new ArrayList<>();
            String nextRow = s.next();
            for (int j = 0; j < numCols; ++j) {
                currentRow.add(nextRow.charAt(j));
            }
            M.add(currentRow);
        }

        s.close();

        path = astar(M, rowPacman, colPacman, rowFood, colFood);
        printPath(path);
    }
}

