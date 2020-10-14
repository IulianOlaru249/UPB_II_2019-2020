#ifndef __PIECES_H__
#define __PIECES_H__

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>


using namespace std;

/* File tolog the board that is stored in memory. 
 * You can see the board state after each move.
 */

extern ofstream log;

enum white_pieces {white_pawn = 1, 
                white_knight = 2,
                white_king = 3,
                white_bishop = 5, 
                white_rook = 6, 
                white_queen = 7};

enum black_pieces {black_pawn = -1, 
                black_knight = -2,
                black_king = -3,
                black_bishop = -5, 
                black_rook = -6,    
                black_queen = -7};
                
struct piece_entry {
        int piece_value;     /* 1=pawn, ..., 7=queen */
        int piece_location;  /* where the piece is on the board */
		bool captured = false;
		vector<int> possible_moves; /* store all possible moves for a piece */
		int next_position;
        int prev_position;
      };

/* white_piece_entries and black_piece_entries are 2 arrays
 * that store the pieces for each side. It is better to have 
 * the pieces as diffrent entities so we have only 32 iterationsos
 * to access all pieces instead of 128 to go trough each square 
 * on the board.
 */

extern piece_entry white_piece_entries[];     
extern piece_entry black_piece_entries[];  

bool is_sliding(int piece_value);
bool is_sliding_diagonally(int piece_value);
bool is_sliding_vertically(int piece_value);
void log_moves(piece_entry *piece);

#endif
