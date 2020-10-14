#include "pieces.h"

ofstream log;

piece_entry white_piece_entries[16];
piece_entry black_piece_entries[16];

/* beacause of the way the pieces are numbered, we can determine 
 *their nature with the below operations
 */

bool is_sliding(int piece) {return (piece & 4) != 0;}
bool is_sliding_diagonally(int piece) {return (((piece & 4) != 0) && ((piece & 1) != 0));}
bool is_sliding_vertically(int piece) {return (piece & 2) != 0; }

void log_moves(piece_entry *piece) {

	log.open("log.txt",std::ios_base::app);

	switch (piece->piece_value)
	{
		case 1:
			log << "White pawn: ";
			break;
		case 2:
			log << "White knight: ";
			break;
		case 3:
			log << "White king: ";
			break;
		case 5:
			log << "White bishop: ";
			break;
		case 6:
			log << "White rook: ";
			break;
		case 7:
			log << "White queen: ";
			break;
	}

	switch (piece->piece_value)
	{
		case -1:
			log << "Black pawn: ";
			break;
		case -2:
			log << "Black knight: ";
			break;
		case -3:
			log << "Black king: ";
			break;
		case -5:
			log << "Black bishop: ";
			break;
		case -6:
			log << "Black rook: ";
			break;
		case -7:
			log << "Black queen: ";
			break;
	}
	
    /* log all valid moves of a piece */
	if (piece != NULL && piece->captured == 0) {
		if ((piece->possible_moves).size() != 0) {
			for(int move : piece->possible_moves) {
				log << move << " ";
			}
			log << endl;
		}
        else {
			log << "No possible moves\n";
        }
	}
    else {
        log << "No possible moves\n";
    }

	log.close();
}
