// skel PA 2017
// Darius - attempt 2018

#include <iostream>
#include <vector>
using namespace std;

#define INF (1 << 30)

/**
 * Reprezinta o mutare efectuata de un jucator
 */
class Move {
public:
  int amount; /* Cantitatea extrasa (1, 2 sau 3) */
  int heap;   /* Indicile multimii din care se face extragerea */

  Move(int amount, int heap);
};

/**
 * Reprezinta starea jocului
 */
class Nim {
public:
  // cele 3 gramezi
  int heaps[3];

  Nim();

  /**
   * Returneaza o lista cu mutarile posibile
   * care pot fi efectuate de player
   */
  vector<Move> get_moves(int player);

  /**
   * Intoarce true daca jocul este intr-o stare finala
   */
  bool ended();

  /**
   * Functia de evaluare a starii curente a jocului
   * Evaluarea se face din perspectiva jucatorului
   * aflat curent la mutare (player)
   */
  int eval(int player);

  /**
   * Aplica o mutarea a jucatorului asupra starii curente
   * Returneaza false daca mutarea e invalida
   */
  bool apply_move(const Move &move);

  /**
   * Afiseaza starea jocului
   */
  void print();
};
