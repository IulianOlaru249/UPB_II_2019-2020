// skel PA 2017
// Darius - attempt 2018

#include "nim.h"

// miscarea care presupune sa scot amount din gramada cu indicele heap
Move::Move(int amount = 0, int heap = -1) : amount(amount), heap(heap) {}

Nim::Nim() {
  heaps[0] = 3;
  heaps[1] = 4;
  heaps[2] = 5;
}

/**
 * Returneaza o lista cu mutarile posibile
 * care pot fi efectuate de player
 */
vector<Move> Nim::get_moves(int player) {
  vector<Move> ret;
  for (int i = 0; i < 3; i++)
    for (int k = 1; k <= 3; k++)
      if (k <= heaps[i])
        ret.push_back(Move(k, i));
  return ret;
}

/**
 * Intoarce true daca jocul este intr-o stare finala
 */
bool Nim::ended() {
  /**
   * TODO Determinati daca jocul s-a terminat
   */

  for (int i = 0; i < 3; ++i) {
    if (heaps[i] > 0) {
      return false;
    }
  }

  return true;
}

/**
 * Functia de evaluare a starii curente a jocului
 * Evaluarea se face din perspectiva jucatorului
 * aflat curent la mutare (player)
 */
int Nim::eval(int player) {
  /**
   * TODO Implementati o functie de evaluare
   * pentru starea curenta a jocului
   *
   * Aceasta trebuie sa intoarca:
   * INF daca jocul este terminat in favoarea lui player
   * -INF daca jocul este terminat in defavoarea lui player
   *
   * In celelalte cazuri ar trebui sa intoarca un scor cu atat
   * mai mare, cu cat player ar avea o sansa mai mare de castig
   */

  // jocul este terminat in favoarea lui player
  if (ended()) {
    return +INF;
  }

  int sum = 0;
  for (int i = 0; i < 3; ++i) {
    sum ^= heaps[i];
  }

  // adversarul are strategie sigura daca suma XOR este 0
  if (!sum) {
    return -INF;
  }

  return 0;
}

/**
 * Aplica o mutarea a jucatorului asupra starii curente
 * Returneaza false daca mutarea e invalida
 */
bool Nim::apply_move(const Move &move) {
  /**
   * TODO Realizati efectuarea mutarii
   * (scadeti move.amount din multimea corespunzatoare,
   *  DACA se poate)
   */

  // daca indexul este invalid, refuza miscarea
  if (move.heap < 0 || move.heap > 2) {
    return false;
  }

  // daca in gramada nu mai sunt suficiente pietre, nu poti face mutarea
  if (heaps[move.heap] < move.amount) {
    return false;
  }

  heaps[move.heap] -= move.amount;
  return true;
}

/**
 * Afiseaza starea jocului
 */
void Nim::print() {
  for (int i = 0; i < 3; i++) {
    cout << i << ":";
    for (int j = 0; j < heaps[i]; j++) {
      cout << " *";
    }
    cout << "\n";
  }
  cout << "\n";
}

/**
 * Implementarea algoritmului minimax (negamax)
 * Intoarce o pereche <best_score, best_move> unde
 *      - best_score este cel mai bun scor care poate
 *  fi obtinut de jucatorul aflat la mutare
 *      - best_move este mutarea propriu-zisa (asociata cu best_score)
 */
pair<int, Move> minimax(Nim game, int player, int depth) {
  /**
   * TODO Implementati conditia de oprire
   * Daca nu aveti o anumita mutare de returnat puteti folosi un obiect dummy =>
   * Move()
   */
  if (game.ended() || depth == 0) {
    return {game.eval(player), Move()};
  }

  vector<Move> moves = game.get_moves(player);
  int best_score = -INF;
  Move best_move = Move(); // dummy move

  /**
   * TODO Determinati cel mai bun scor si cea mai buna mutare
   * folosind algoritmul minimax
   */
  for (auto &move : moves) {
    Nim new_game = game;

    if (new_game.apply_move(move)) {
      auto res = minimax(new_game, -player, depth - 1);
      int score = -res.first;

      if (score >= best_score) {
        best_score = score;
        best_move = move;
      }
    }
  }

  // returnam pereche: cel mai bun score, mutare aferenta
  return {best_score, best_move};
}

void legend() {
  cout << "legend: player == +1  => bot minimax\n";
  cout << "legend: player == -1  => student or other bot (default student)\n";
}

int main() {
  // hardcodam un joc.. why not?
  Nim nim;

  nim.heaps[0] = 5;
  nim.heaps[1] = 10;
  nim.heaps[2] = 20;

  // afisam starea initiala
  nim.print();
  legend();

  // Choose here if you want COMP vs HUMAN
  //                      or COMP vs COMP
  bool HUMAN_PLAYER = false;
  int player = 1;

  // jucam pana la game over
  while (!nim.ended()) {
    pair<int, Move> p;
    if (player == 1) {
      // ne asteptam sa intre pana la adancime 6 intr-un timp rezonabil
      p = minimax(nim, player, 6);

      cout << "Player " << player << " (minimax) evaluates to "
           << "score: " << p.first << "; amount: " << p.second.amount
           << "; heap: " << p.second.heap << '\n';
      nim.apply_move(p.second);
    } else {
      // daca nu joaca utilizatorul, atunci voi pune alt bot sa joace cu primul
      // bot/
      if (!HUMAN_PLAYER) {
        // si botul asta va folosi minimax
        // ne asteptam sa intre pana la adancime 6 intr-un timp rezonabil
        p = minimax(nim, player, 6);

        cout << "Player " << player << " (OTHER minimax) evaluates to "
             << "score: " << p.first << "; amount: " << p.second.amount
             << "; heap: " << p.second.heap << '\n';
        nim.apply_move(p.second);
      } else {
        // joaca userul - asta e dummy si poate introdice valori gresite
        // verifica ca introduce ce trebuie
        cout << "Go player " << player << " (student) !\n";
        bool valid = false;
        int amount, heap;

        while (!valid) {
          cout << "Insert amount [1, 2 or 3] and heap [0, 1 or 2]: ";
          cin >> amount >> heap;

          if (amount >= 1 && amount <= 3 && heap >= 0 && heap <= 2) {
            valid = true;
          } else {
            cout << "\t\t\t\t\t\t\tTry again ... (something is invalid)\n";
          }
        }

        nim.apply_move(Move(amount, heap));
      }
    }

    // printeaza starea curenta. Sesizati?
    nim.print();

    // tura urmatoare muta celalalt player
    player *= -1;
  }

  // s-a terminat jocul si adversarul a luat chestie => EU castig
  // (-player e cel care a facut ultima mutare in while!)
  cout << "Player " << player << " WON!" << endl;
  legend();

  return 0;
}
