// skel PA 2017

#include "reversi.h"

Move::Move(int player, int x = -1, int y = -1) : player(player), x(x), y(y) {}

Reversi::Reversi() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      data[i][j] = 0;
    }
  }

  data[N / 2 - 1][N / 2 - 1] = data[N / 2][N / 2] = 1;
  data[N / 2 - 1][N / 2] = data[N / 2][N / 2 - 1] = -1;
}

/**
    * Returneaza o lista cu mutarile posibile
    * care pot fi efectuate de player
    */
vector<Move> Reversi::get_moves(int player) {
  vector<Move> moves;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      moves.push_back(Move(player, i, j));
    }
  }

  return moves;
}

/**
 * Intoarce true daca jocul este intr-o stare finala
 */
bool Reversi::ended() {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      Reversi tmp(*this);
      if (tmp.apply_move(Move(1, i, j)))
        return false;
      tmp = *this;
      if (tmp.apply_move(Move(-1, i, j)))
        return false;
    }
  return true;
}

/**
 * Returns 1 if player won, 0 if draw and -1 if lost
 */
int Reversi::winner(int player) {
  if (!ended())
    return 0;

  int s = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      if (data[i][j] == player)
        s++;
      else if (data[i][j] == -player)
        s--;
    }
  return s > 0 ? 1 : s == 0 ? 0 : -1;
}

/**
 * Functia de evaluare a starii curente a jocului
 * Evaluarea se face din perspectiva jucatorului
 * aflat curent la mutare (player)
 */
int Reversi::eval(int player) {
  /**
   * TODO Implementati o functie de evaluare
   * Aceasta trebuie sa intoarca:
   * INF daca jocul este terminat in favoarea lui player
   * -INF daca jocul este terminat in defavoarea lui player
   *
   * In celelalte cazuri ar trebui sa intoarca un scor cu atat
   * mai mare, cu cat player ar avea o sansa mai mare de castig
   */

  if (ended()) {
    return winner(player) * INF;
  }

  int s = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (data[i][j] == player) {
        s++;
      } else if (data[i][j] == -player) {
        s--;
      }
    }
  }

  return s;
}

/**
 * Aplica o mutare a jucatorului, modificand starea jocului
 * Plaseaza piesa jucatorului move.player in pozitia move.x, move.y
 * Mutarea move.x == -1, move.y == -1 semnifica ca jucatorul
 * paseaza randul la mutare
 * Returneaza true daca mutarea este valida
 */
bool Reversi::apply_move(const Move &move) {
  if (move.x == -1 && move.y == -1)
    return true;

  if (data[move.x][move.y] != 0)
    return false;

  bool ok = false;

  for (int x_dir = -1; x_dir <= 1; x_dir++)
    for (int y_dir = -1; y_dir <= 1; y_dir++) {
      if (x_dir == 0 && y_dir == 0)
        continue;
      int i = move.x + x_dir, j = move.y + y_dir;
      for (; i >= 0 && j >= 0 && i < N && j < N && data[i][j] == -move.player;
           i += x_dir, j += y_dir)
        ;
      if (i >= 0 && j >= 0 && i < N && j < N && data[i][j] == move.player &&
          (abs(move.x - i) > 1 || abs(move.y - j) > 1)) {
        ok = true;
        for (i = move.x + x_dir, j = move.y + y_dir;
             i >= 0 && j >= 0 && i < N && j < N && data[i][j] == -move.player;
             i += x_dir, j += y_dir)
          data[i][j] = move.player;
      }
    }

  if (!ok)
    return false;

  data[move.x][move.y] = move.player;

  return true;
}

/**
 * Afiseaza starea jocului
 */
void Reversi::print() {
  cout << "N = " << N << "\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (data[i][j] == 0)
        cout << '.';
      else if (data[i][j] == 1)
        cout << 'O';
      else
        cout << 'X';
      cout << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}

/**
 * Implementarea algoritmului minimax (negamax)
 * Intoarce o pereche <best_score, best_move> unde best_score este cel mai bun
 * scor
 * care poate fi obtinut de jucatorul aflat la mutare,
 * iar best_move este mutarea propriu-zisa
 */
pair<int, Move> minimax(Reversi game, int player, int depth) {
  /**
   * TODO Implementati conditia de oprire
   */
  if (game.ended() || depth == 0) {
    return {game.eval(player), Move(player)};
  }

  /**
   * TODO Determinati mutarile posibile
   * si determinati cel mai bun scor si cea mai buna mutare
   * folosind algoritmul minimax
   *
   * Fiti atenti ca daca player nu poate efectua nici o mutare, el
   * poate ceda randul la mutat prin mutarea conventionala <player, -1, -1>
   */
  vector<Move> moves = game.get_moves(player);
  int best_score = -INF;
  Move best_move = Move(player, -1, -1);

  for (auto &move : moves) {
    Reversi new_game = game;

    if (new_game.apply_move(move)) {
      auto res = minimax(new_game, -player, depth - 1);
      int score = -res.first;

      if (score >= best_score) {
        best_score = score;
        best_move = move;
      }
    }
  }


  return {best_score, best_move};
}

/**
 * Implementarea de negamax cu alpha-beta pruning
 * Intoarce o pereche <best_score, best_move> unde best_score este cel mai bun
 * scor
 * care poate fi obtinut de jucatorul aflat la mutare,
 * iar best_move este mutarea propriu-zisa
 */
pair<int, Move> minimax_abeta(Reversi game, int player, int depth, int alpha,
                              int beta) {
  /* Conditia de oprire */
  if (game.ended() || depth == 0) {
    return {game.eval(player), Move(player)};
  }

  /**
   * TODO Determinati mutarile posibile
   * si determinati cel mai bun scor si cea mai buna mutare
   * folosind algoritmul minimax cu alfa-beta pruning
   *
   * Fiti atenti ca daca player nu poate efectua nici o mutare, el
   * poate ceda randul la mutat prin mutarea conventionala <player, -1, -1>
   */

  vector<Move> moves = game.get_moves(player);
  int best_score = -INF;
  Move best_move = Move(player, -1, -1);

  for (auto &move : moves) {
    Reversi new_game = game;

    if (new_game.apply_move(move)) {
      auto res = minimax_abeta(new_game, -player, depth - 1, -beta, -alpha);
      int score = -res.first;

      if (alpha <= score && score <= beta && score >= best_score) {
        best_score = score;
        best_move = move;
      }

      if (score > alpha) {
        alpha = score;
      }

      if (alpha >= beta) {
        break;
      }
    }
  }

  return {best_score, best_move};
}

void legend() {
  cout << "legend: player == +1 => O (a.k.a. minimax)\n";
  cout << "legend: player == -1 => X (a.k.a student of other bot)\n";
}

int main() {
  // un jov de reversi
  Reversi rev;

  // afisam starea initiala
  rev.print();
  legend();

  // Choose here if you want COMP vs HUMAN
  //                      or COMP vs COMP
  bool HUMAN_PLAYER = false;
  int player = 1;

  while (!rev.ended()) {
    pair<int, Move> p = {0, Move(player)};

    if (player == 1) {
      p = minimax(rev, player, 6);
      // p = minimax_abeta(rev, player, 9, -INF, INF);

      cout << "Player " << player << " O (minimax) evaluates to "
           << " score: " << p.first << " x, y: " << p.second.x << ","
           << p.second.y << "\n";
      rev.apply_move(p.second);
    } else {
      if (!HUMAN_PLAYER) {
        // p = minimax(rev, player, 6);
        p = minimax_abeta(rev, player, 9, -INF, INF);

        cout << "Player " << player
             << " X (a.k.a. student or other bot) evaluates to "
             << " score: " << p.first << " x, y: " << p.second.x << ","
             << p.second.y << "\n";
        rev.apply_move(p.second);
      } else {
        cout << "Go player " << player << " (student) !\n";

        bool valid = false;
        while (!valid) {
          int x, y;
          cout << "Insert position [0..N - 1], [0..N - 1] ";
          cin >> x >> y;
          valid = rev.apply_move(Move(player, x, y));
        }
      }
    }

    rev.print();
    player *= -1;
  }

  int w = rev.winner(1);
  if (w == 1)
    cout << "Player 1 WON!" << endl;
  else if (w == 0)
    cout << "Draw" << endl;
  else
    cout << "Player -1 WON!" << endl;

  legend();
}
