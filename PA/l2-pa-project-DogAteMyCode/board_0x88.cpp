#include "board_0x88.h"
#include <string> 

piece_entry *board[BOARD_SIZE] = {NULL};

/* this function initializes all white pieces and stores them in array */
void init_white_pieces () {

	int i = 0, pawn_position = 16;
	for (i = 0; i < RANK; i++){
		white_piece_entries[i] = {white_pawn, pawn_position, false};
		pawn_position++;
	}
	
	piece_entry rook = {white_rook, 0, false};
	white_piece_entries[i] = rook;
	i++;

	piece_entry knight = {white_knight, 1, false};
	white_piece_entries[i] = knight;
	i++;

	piece_entry bishop = {white_bishop, 2, false};
	white_piece_entries[i] = bishop;
	i++;

	piece_entry queen = {white_queen, 3, false};
	white_piece_entries[i] = queen;
	i++;

	piece_entry king = {white_king, 4, false};
	white_piece_entries[i] = king;
	i++;

	piece_entry bishop_2 = {white_bishop, 5, false};
	white_piece_entries[i] = bishop_2;
	i++;

	piece_entry knight_2 = {white_knight, 6, false};
	white_piece_entries[i] = knight_2;
	i++;

	piece_entry rook_2 = {white_rook, 7, false};
	white_piece_entries[i] = rook_2;
}

/* this function initializes all black pieces and stores them in array */
void init_black_pieces () {
	int i = 0,  pawn_position = 96;

	for (i = 0; i < RANK; i++){
		black_piece_entries[i] = {black_pawn, pawn_position, false};
		pawn_position ++;
	}
	

	piece_entry rook = {black_rook, 112, false};
	black_piece_entries[i] = rook;
	i++;

	piece_entry knight = {black_knight, 113, false};
	black_piece_entries[i] = knight;
	i++;

	piece_entry bishop = {black_bishop, 114, false};
	black_piece_entries[i] = bishop;
	i++;

	piece_entry queen = {black_queen, 115, false};
	black_piece_entries[i] = queen;
	i++;

	piece_entry king = {black_king, 116, false};
	black_piece_entries[i] = king;
	i++;

	piece_entry bishop_2 = {black_bishop, 117, false};
	black_piece_entries[i] = bishop_2;
	i++;

	piece_entry knight_2 = {black_knight, 118, false};
	black_piece_entries[i] = knight_2;
	i++;

	piece_entry rook_2 = {black_rook, 119, false};
	black_piece_entries[i] = rook_2;		
}

/* Store the board in a log file */
void print_board() {
	log.open("log.txt",std::ios_base::app);

	log << endl;
    int j = 112;
    while (j >= 0) {
	    for (int file = j; file < j + 8; file++){
	        if (board[file] != NULL)
		        if (board[file]->piece_value > 0)
			 	    log << " " << board[file]->piece_value << "  ";
				else 
			 	    log << board[file]->piece_value << "  ";
			 else
			    log << " 0  ";
	    }
        j = j - 16;
        log << endl;
    }

	log.close();
}

void set_board () {

	int j = 8;
	/* place white rooks, knights, bishops, king, queen on board */
	for (int i = 0; i < RANK; i++){
		board[i] = &white_piece_entries[j];
		j++;
	}

	/* place white pawns on board */
	j = 0;
	for (int i = 2 * RANK; i < 3 * RANK; i++){
		board[i] = &white_piece_entries[j];
		j++;
	}

	/* place black pawns on board */
	j = 0;
	for (int i = 12 * RANK; i < 13 * RANK; i++){
		board[i] = &black_piece_entries[j];
		j++;
	}

	/* place black rooks, knights, bishops, king, queen on board */
	for (int i = 14 * RANK; i < 15 * RANK; i++){
		board[i] = &black_piece_entries[j];
		j++;
	}
}

void update_board (char *move) {

	/* store the start position of piece */
	char next_opponent_position[3];
	next_opponent_position[0]  = move[0];
	next_opponent_position[1] = move[1];
	next_opponent_position[2] = '\0';

	/* update the next location of piece */
	int position = get_square_num(next_opponent_position);
	board[position]->next_position = get_square_num(move + 2);

	/* if there is a piece of diffrent color on that sqare,
	 * mark it as captured.
	 */	
	if (board[board[position]->next_position] != NULL) {
		if (((board[board[position]->next_position]->piece_value * 
				(board[position])->piece_value) < 0)){
			(board[board[position]->next_position])->captured = true; 
		}
	}

	if (move[4] == 'q') {
		board[position]->piece_value *= 7;
	}

	board[position]->prev_position = board[position]->piece_location;
	/*Updating current position of the piece with the next location*/
	board[position]->piece_location = board[position]->next_position;
	/* put the piece on the square */
	board[board[position]->next_position] = board[position];
	/* mark square that the piece was on as empty */ 
	board[position] = NULL;
}

/* transform a pair of positions in a command (eg 16 32 -> a6a7) */
void get_square_code(char *current_move, int piece_location, int next_location) {
    char character;
    int number;

    number = (piece_location / RANK) / 2 + 1;
    character = (piece_location) % RANK + 97;
    
    current_move[0] = character;
    current_move[1] = number + '0';

    number = (next_location / RANK) / 2 + 1;
    character = (next_location) % RANK + 97;

    current_move[2] = character;
    current_move[3] = number + '0';
	current_move[4] = '\0';
}

/* transform a move in a position on board */
int get_square_num(char *secv){
	if (secv == NULL)
		return 0;

	int location = 0, num, ch;

	ch = secv[0] - 97;
	location = ch;
	num = (secv[1] - '0' - 1) * 2 * RANK;
	location += num;

	return location;
}
