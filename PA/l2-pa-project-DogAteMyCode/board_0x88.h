/* 
 * 0x88 method to represent the chess board in memory.
 */

#ifndef __BOARD_0X88_H__
#define __BOARD_0X88_H__

#include "pieces.h"

#define BOARD_SIZE 128
#define RANK 8

extern piece_entry *board[];
extern char current_move[];

void init_white_pieces();
void init_black_pieces();
void update_board(char *move);
void set_board();
void print_board();
void get_square_code(char *current_move, int piece_location, int next_location);
int get_square_num(char *secv);

//maybe later
//piece_entry get_piece();

#endif
