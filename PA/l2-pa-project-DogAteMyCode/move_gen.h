#ifndef __MOVE_GEN_H__
#define __MOVE_GEN_H__

#include "board_0x88.h"

/* the knight can only go to some offsets, based on its current positions */
extern int knight_offsets[];
extern int king_offsets[];

int generate_moves_knight(piece_entry *knight);
int generate_moves_bishop (piece_entry *bishop);
int generate_moves_rook (piece_entry *rook);
int generate_moves_queen (piece_entry *queen);
int generate_moves_pawn (piece_entry *pawn);
int generate_moves_king (piece_entry *king);
bool check_chess(int piece_location, int color);

#endif
