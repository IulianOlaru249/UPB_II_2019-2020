#include "move_gen.h"

int knight_offsets[] = {2 * 16 - 1,
				2 * 16 + 1,
				-2 * 16 - 1,
				-2 * 16 + 1,
				16 + 2,
				16 - 2,
				-16 + 2,
				-16 - 2};

int king_offsets[] = {1, -1, 16, -16, -17, -15, 17, 15};

bool is_king_alligned (int piece_location, int color) 
{
    int king_location;
    if (color < 0)
        king_location = black_piece_entries[12].piece_location;
    else
        king_location = white_piece_entries[12].piece_location;

    /* on the same horizontal line */
    if (abs (piece_location - king_location) <= 7) {
        return true;
    }

    /* on the same vertical line */
    if (abs (piece_location - king_location) % 16 == 0) {
        return true;
    }

    /* on same diagonal */
    if (abs (piece_location - king_location) % 15 == 0) {
        return true;
    }
    if (abs (piece_location - king_location) % 17 == 0) {
        return true;
    }

    return false;
}

bool check_chess(int piece_location, int color) {
    // log.open("log.txt",std::ios_base::app);
    // log << "start check" << endl;
    // log.close();

    /*Check if king is safe on diagonals */
    int test_move_up_right = piece_location;
    int test_move_up_left = piece_location;
    int test_move_down_right = piece_location;
    int test_move_down_left = piece_location;

    bool is_done_up_right = false;
    bool is_done_up_left = false;
    bool is_done_down_right = false;
    bool is_done_down_left = false;

    do {

        /* Move up right diagonally */
         test_move_up_right += 17;
        test_move_up_left += 15;
        test_move_down_right -= 15;
        test_move_down_left -= 17;

        // test_move_up_right += 17;
        // test_move_up_left += 15;
        // test_move_down_right -= 15;
        // test_move_down_left -= 17;

        /* If we are out of the board */
        if ((test_move_up_right & 0x88) != 0)
            is_done_up_right = true;

        if ((test_move_up_left & 0x88) != 0)
            is_done_up_left = true;

        if ((test_move_down_right & 0x88) != 0)
            is_done_down_right = true;

        if ((test_move_down_left & 0x88) != 0)
            is_done_down_left = true;

        if (!is_done_up_right) {
            if ( board[test_move_up_right] != NULL ) {
                if ((( board[test_move_up_right]->piece_value * color) < 0)){
                    if(board[test_move_up_right]->piece_value == -1 * color * 7 || 
                            board[test_move_up_right]->piece_value == -1 * color * 5){
                            return false;
                        }
                    /* If piece can't slide diagonally it protects the king on that line no matter the color */
                    else
                        is_done_up_right = true;
                }
                /* After a piece of the same color is found there is no point in looking further
                 * on that line since the king will be protected.
                 */
                else {
                    is_done_up_right = true;
                }
            }
        }

        if (!is_done_up_left) {
            if ( board[test_move_up_left] != NULL ) {
                if ((( board[test_move_up_left]->piece_value * color) < 0)){
                        if(board[test_move_up_left]->piece_value == -1 * color * 7 || 
                            board[test_move_up_left]->piece_value == -1 * color * 5){
                                return false;
                            }
                        else
                            is_done_up_left = true;
                }
                else {
                    is_done_up_left = true;
                }
            }
        }

        if (!is_done_down_right) {
            if ( board[test_move_down_right] != NULL ) {
                if ((( board[test_move_down_right]->piece_value * color) < 0)){
                        if(board[test_move_down_right]->piece_value == -1 * color * 7 || 
                            board[test_move_down_right]->piece_value == -1 * color * 5) {
                                return false;
                            }
                        else
                            is_done_down_right = true;
                        
                }
                else {
                    is_done_down_right = true;
                }
            }
        }

        if (!is_done_down_left) {
            if ( board[test_move_down_left] != NULL ) {
                if ((( board[test_move_down_left]->piece_value * color) < 0)){
                    if(board[test_move_down_left]->piece_value == -1 * color * 7 || 
                        board[test_move_down_left]->piece_value == -1 * color * 5){
                            return false;
                        }
                    else
                        is_done_down_left = true;
                }
                else {
                    is_done_down_left = true;
                }
            }
        }

        if (is_done_up_right && is_done_up_left && 
                is_done_down_right && is_done_down_left)
            break;

    }while(true);
    
    int test_move_up = piece_location;
    int test_move_right = piece_location;
    int test_move_down = piece_location;
    int test_move_left = piece_location;
 
    bool is_done_up = false;
    bool is_done_down = false;
    bool is_done_right = false;
    bool is_done_left = false;

    do {

        test_move_up += 16;
        test_move_right += 1;
        test_move_down -= 16;
        test_move_left -= 1;
 
        /* If we are out of the board */
        if ((test_move_up & 0x88) != 0)
            is_done_up = true;
 
        if ((test_move_right & 0x88) != 0)
            is_done_right = true;
 
        if ((test_move_down & 0x88) != 0)
            is_done_down = true;
 
        if ((test_move_left & 0x88) != 0)
            is_done_left = true;
 
        /* Generate up */
        if (!is_done_up) {
            if ( board[test_move_up] != NULL ) {
                if ((( board[test_move_up]->piece_value * color) < 0)){
                        if(board[test_move_up]->piece_value == -1 * color * 7 || 
                            board[test_move_up]->piece_value == -1 * color * 6) {
                                return false;
                            }
                        else 
                                is_done_up = true;
                    }
                    else {
                        is_done_up = true;
                    }
                }
            }

        if (!is_done_down) {
            if ( board[test_move_down] != NULL ) {
                if ((( board[test_move_down]->piece_value * color) < 0)){
                        if(board[test_move_down]->piece_value == -1 * color * 7 || 
                            board[test_move_down]->piece_value == -1 * color * 6) {
                                return false;
                            }
                        else 
                                is_done_down = true;
                    }
                    else {
                        is_done_down = true;
                    }
                }
            }

        if (!is_done_right) {
            if ( board[test_move_right] != NULL ) {
                if ((( board[test_move_right]->piece_value * color) < 0)){
                        if(board[test_move_right]->piece_value == -1 * color * 7 || 
                            board[test_move_right]->piece_value == -1 * color * 6){
                                return false;
                            }
                        else 
                                is_done_right = true;
                    }
                    else {
                        is_done_right = true;
                    }
                }
            }

        if (!is_done_left) {
            if ( board[test_move_left] != NULL ) {
                if ((( board[test_move_left]->piece_value * color) < 0)){
                        if(board[test_move_left]->piece_value == -1 * color * 7 || 
                            board[test_move_left]->piece_value == -1 * color * 6){
                                return false;
                            }
                        else 
                                is_done_left = true;
                    }
                    else {
                        is_done_left = true;
                    }
                }
            }
 
        if (is_done_up && is_done_right &&
                is_done_down && is_done_left)
            break;

    } while (true);

    if (((piece_location + 15 * color) & 0x88) == 0) {
        if(board[piece_location + 15 * color] != NULL) {
            if(board[piece_location + 15 * color]->piece_value == (-1) * color) {
                return false;
            }
        }
    }
    
    if (((piece_location + 17 * color) & 0x88) == 0) {
        if(board[piece_location + 17 * color] != NULL) {
            if(board[piece_location + 17 * color]->piece_value == (-1) * color) {
                return false;
            }
        }
    }

    int test_move;
    for(int i = 0; i < 8; i++) {
        test_move = piece_location + knight_offsets[i];
        if ((test_move & 0x88) == 0 )
        {
            if(board[test_move] != NULL) {
                if(board[test_move]->piece_value == (-1) * color * 2) {
                    return false;
                }
            }
        }
    }

    return true;
}

int generate_moves_king (piece_entry *king)
{
    int number_of_moves = 0;
    (king->possible_moves).clear();
    int color = king->piece_value > 0 ? 1 : -1;

    if (king == NULL || king->captured == true)
        return number_of_moves;

    for (int i = 0; i < 8; i++) {
       // piece_entry *old_king = board[king->piece_location];
       // board[king->piece_location] = NULL;

        int test_move =  king->piece_location + king_offsets[i];
        
		if ((test_move & 0x88) == 0 ) {
            if (board[test_move] != NULL) {
                if ((( board[test_move]->piece_value * king->piece_value) < 0)){
                    //if not in check
                    piece_entry *aux = board[king->piece_location];
                    piece_entry *aux2 = board[test_move];
                    board[test_move] = aux;
                    board[king->piece_location] = NULL;

                    /*Check for king around the other king*/

                    bool is_valid = true;
                    int diff;

                    if (color < 0)
                    {
                        diff = abs(white_piece_entries[12].piece_location - test_move);
                    }
                    else {
                        diff = abs(black_piece_entries[12].piece_location - test_move);
                    }

                    if((diff >= 15 && diff <= 17) || (diff <= 1)) {
                        is_valid = false;
                    }

                    if(check_chess(test_move, color) && is_valid) {
                        (king->possible_moves).push_back(test_move);
                        number_of_moves ++;
                    }

                    board[test_move] = aux2;
                    board[king->piece_location] = aux;
                }
            }
            else {
                //if not in check
                piece_entry *aux = board[king->piece_location];
                board[test_move] = aux;
                board[king->piece_location] = NULL;

                bool is_valid = true;

                int diff;

                if (color < 0)
                {
                    diff = abs(white_piece_entries[12].piece_location - test_move);
                }
                else {
                    diff = abs(black_piece_entries[12].piece_location - test_move);
                }

                if((diff >= 15 && diff <= 17) || (diff <= 1)) {
                    is_valid = false;
                }

                if(check_chess(test_move, color) && is_valid) {
                    (king->possible_moves).push_back(test_move);
                    number_of_moves ++;
                }

                board[test_move] = NULL;
                board[king->piece_location] = aux;
            }
        }

       // board[king->piece_location] = old_king;        
    }

    return number_of_moves;
}

int generate_moves_knight(piece_entry *knight) {

    int number_of_moves = 0;

    (knight->possible_moves).clear();
    int color = (knight->piece_value < 0) ? -1 : 1;

	if (knight == NULL || knight->captured == true)	{
		return number_of_moves;
	}

     piece_entry * king;
    if (color < 0) {
        king = &black_piece_entries[12];
    }
    else {
        king = &white_piece_entries[12];
    }
	
	int test_move;
	for (int i = 0; i < 8; i++) {
		
		test_move = knight->piece_location + knight_offsets[i];

		if ((test_move & 0x88) == 0 ) {

			/* the move is valid if the square is on the board and if there is 
			 *no piece of the same color on that square
   			 */
			if ( board[test_move] != NULL ) {
				if ((( board[test_move]->piece_value * knight->piece_value) < 0)){
                    /*generam mutarea, facem mutarea ipotetica, verificam daca regele e inca in sah */
                    piece_entry *aux = board[knight->piece_location];
                    piece_entry *aux2 = board[test_move];
                    board[test_move] = aux;
                    board[knight->piece_location] = NULL;

                    if (check_chess(king->piece_location, color)) {
					    (knight->possible_moves).push_back(test_move);
                        number_of_moves ++;
                    }

                    board[test_move] = aux2;
                    board[knight->piece_location] = aux;
				}
			}
			else {
                piece_entry *aux = board[knight->piece_location];
                board[knight->piece_location] = NULL;
                board[test_move] = aux;

                if (check_chess(king->piece_location, color)) {
				    (knight->possible_moves).push_back(test_move);
                    number_of_moves ++;
                }

                board[knight->piece_location] = aux;
                board[test_move] = NULL;
			}
		}
	}

    return number_of_moves;
}

bool check_path (piece_entry *piece, int test_move)
{
    int color = piece->piece_value < 0 ? -1 : 1;
    bool is_done = false;

    piece_entry * king;
    if (color < 0)
        king = &black_piece_entries[12];
    else
        king = &white_piece_entries[12];

    if ( board[test_move] != NULL ) {
        if ((( board[test_move]->piece_value * piece->piece_value) < 0)){
            /* If the king is not on the same diagonal,  there is no worry of chess */
            /* make hypotetical move on board */
                piece_entry *aux = board[piece->piece_location];
                piece_entry * aux2 = board[test_move];
                board[test_move] = aux;
                board[piece->piece_location] = NULL;
            
                /* check if king is safe after the move */
                if (!check_chess(king->piece_location, color)) {
                    is_done = true;
                }
                /* the king is safe after the piece is taken */
                else {
                    (piece->possible_moves).push_back(test_move);
                    is_done = true;
                }

                /* restore board to originalstate */
                board[piece->piece_location] = aux;
                board[test_move] = aux2;
        }
        else {
            is_done = true;
        }
    }
    else {
        //if (is_king_alligned(piece->piece_location, color)) {
            piece_entry *aux = board[piece->piece_location];
            board[piece->piece_location] = NULL;
            board[test_move] = aux;

                /* If king is not left in chess after move */
            piece_entry * king;
            if (color < 0)
                king = &black_piece_entries[12];
            else
                king = &white_piece_entries[12];

            if (check_chess(king->piece_location, color)) {
                (piece->possible_moves).push_back(test_move);
            }
            
            board[piece->piece_location] = aux;
            board[test_move] = NULL;
    }
    return is_done;
}

int generate_moves_diagonally(piece_entry *piece)
{
    int number_of_moves = 0;
    //int color = piece->piece_value < 0 ? -1 : 1;

    int test_move_up_right = piece->piece_location;
    int test_move_up_left = piece->piece_location;
    int test_move_down_right = piece->piece_location;
    int test_move_down_left = piece->piece_location;

    bool is_done_up_right = false;
    bool is_done_up_left = false;
    bool is_done_down_right = false;
    bool is_done_down_left = false;

    do {
        /* Move up right diagonally */
        test_move_up_right += 17;
        test_move_up_left += 15;
        test_move_down_right -= 15;
        test_move_down_left -= 17;

        /* If we are out of the board */
        if ((test_move_up_right & 0x88) != 0)
            is_done_up_right = true;

        if ((test_move_up_left & 0x88) != 0)
            is_done_up_left = true;

        if ((test_move_down_right & 0x88) != 0)
            is_done_down_right = true;

        if ((test_move_down_left & 0x88) != 0)
            is_done_down_left = true;

        /* Generate up right */
        if (!is_done_up_right) {
           is_done_up_right = check_path(piece, test_move_up_right);
           number_of_moves++;
        }
        
        /* Generate up left */
        if (!is_done_up_left) {
            is_done_up_left = check_path(piece, test_move_up_left);
            number_of_moves++;
        }

        /* Generate down right */
        if (!is_done_down_right) {
            is_done_down_right = check_path(piece, test_move_down_right);
            number_of_moves++;
        }
 
        /* Generate down left */
        if (!is_done_down_left) {
            is_done_down_left = check_path(piece, test_move_down_left);
            number_of_moves++;
        }

        if (is_done_up_right && is_done_up_left && 
                is_done_down_right && is_done_down_left)
            break;

    } while (true);

    return number_of_moves;
}

int generate_moves_sliding(piece_entry * piece)
{
    int number_of_moves = 0;

    int test_move_up = piece->piece_location;
    int test_move_right = piece->piece_location;
    int test_move_down = piece->piece_location;
    int test_move_left = piece->piece_location;
 
    bool is_done_up = false;
    bool is_done_down = false;
    bool is_done_right = false;
    bool is_done_left = false;

     do {
        /* Move up right diagonally */
        test_move_up += 16;
        test_move_right += 1;
        test_move_down -= 16;
        test_move_left -= 1;
 
        /* If we are out of the board */
        if ((test_move_up & 0x88) != 0)
            is_done_up = true;
 
        if ((test_move_right & 0x88) != 0)
            is_done_right = true;
 
        if ((test_move_down & 0x88) != 0)
            is_done_down = true;
 
        if ((test_move_left & 0x88) != 0)
            is_done_left = true;
 
        /* Generate up */
        if (!is_done_up) {
            is_done_up = check_path(piece, test_move_up);
            number_of_moves++;
        }
 
        /* Generate right */
        if (!is_done_right) {
            is_done_right = check_path(piece, test_move_right);
            number_of_moves++;
        }
 
        /* Generate down */
        if (!is_done_down) {
            is_done_down = check_path(piece, test_move_down);
            number_of_moves++;
        }
 
        /* Generate left */
        if (!is_done_left) {
            is_done_left = check_path(piece, test_move_left);
            number_of_moves++;
        }
 
        if (is_done_up && is_done_right &&
                is_done_down && is_done_left)
            break;

    } while (true);
 
    return number_of_moves;
}

int generate_moves_bishop (piece_entry *bishop) {
    int number_of_moves = 0;

    (bishop->possible_moves).clear();

    if (bishop == NULL || bishop->captured == true)
        return number_of_moves;

    number_of_moves = generate_moves_diagonally(bishop);

    return number_of_moves;
}

int generate_moves_rook (piece_entry *rook) {
    int number_of_moves = 0;

    (rook->possible_moves).clear();

    if (rook == NULL || rook->captured == true)
         return number_of_moves;

    number_of_moves = generate_moves_sliding (rook);

    return number_of_moves;
}

int generate_moves_queen (piece_entry *queen) {
    int number_of_moves = 0;

    (queen->possible_moves).clear();

    if (queen == NULL || queen->captured == true)
         return number_of_moves;

    number_of_moves += generate_moves_sliding (queen);
    number_of_moves += generate_moves_diagonally (queen);

    return number_of_moves;
} 

int generate_moves_pawn (piece_entry *pawn) {
    int number_of_moves = 0;
    (pawn->possible_moves).clear();

    if (pawn == NULL || pawn->captured == true)
        return number_of_moves;


    int color = pawn->piece_value;
    piece_entry * king;
    if (color < 0)
        king = &black_piece_entries[12];
    else
        king = &white_piece_entries[12];


    //bool moved_once = false;
    /* Check if can move one up*/
    int test_move = pawn->piece_location + (16 * color);
    if (board[test_move] == NULL ) {   
        //if (is_king_alligned(pawn->piece_location, color)) {
            piece_entry *aux = board[pawn->piece_location];
            board[pawn->piece_location] = NULL;
            board[test_move] = aux;


            if (check_chess(king->piece_location, color)) {
                pawn->possible_moves.push_back(test_move);
                number_of_moves ++;
                //moved_once = true;   
            }
            
            board[pawn->piece_location] = aux;
            board[test_move] = NULL;
        /*else {
            pawn->possible_moves.push_back(test_move);
            number_of_moves ++;
            moved_once = true;
        }*/
    }

    /* Check if can move two up white*/
    if (color > 0 && (pawn->piece_location >= 16 && pawn->piece_location <=23)) {
        if (board[pawn->piece_location + 16] == NULL) {
            test_move = pawn->piece_location + 32;
            if (board[test_move] == NULL) {
                piece_entry *aux = board[pawn->piece_location];
                board[pawn->piece_location] = NULL;
                board[test_move] = aux;

                if (check_chess(king->piece_location, color)) {
                    pawn->possible_moves.push_back(test_move);
                    number_of_moves ++;
                    //moved_once = true;   
                }
                
                board[pawn->piece_location] = aux;
                board[test_move] = NULL;
            }
        }
    }

    /*Check if can move two up black*/ 
   if (color < 0 && (pawn->piece_location >= 96 && pawn->piece_location <= 103)) {
        if (board[pawn->piece_location - 16] == NULL) {
            test_move = pawn->piece_location - 32;
            if (board[test_move] == NULL) {
                piece_entry *aux = board[pawn->piece_location];
                board[pawn->piece_location] = NULL;
                board[test_move] = aux;

                if (check_chess(king->piece_location, color)) {
                    pawn->possible_moves.push_back(test_move);
                    number_of_moves ++;
                    //moved_once = true;   
                }
                
                board[pawn->piece_location] = aux;
                board[test_move] = NULL;
            }
        }
    }

    /* Check if can capute piece */
    test_move = pawn->piece_location + (17 * color);
    if (board[test_move] != NULL){
        if ((( board[test_move]->piece_value * color) < 0)){
                /* make hypotetical move on board */
                piece_entry *aux = board[pawn->piece_location];
                piece_entry * aux2 = board[test_move];
                board[test_move] = aux;
                board[pawn->piece_location] = NULL;

                /* check if king is safe after the move */
                if (check_chess(king->piece_location, color)) {
                /* the king is safe after the piece is taken */
                    (pawn->possible_moves).push_back(test_move);
                    number_of_moves ++;
                }

                /* restore board to originalstate */
                board[pawn->piece_location] = aux;
                board[test_move] = aux2;
        }
    }

    test_move = pawn->piece_location + (15 * color);
    if (board[test_move] != NULL){
        if ((( board[test_move]->piece_value * color) < 0)){
                /* make hypotetical move on board */
                piece_entry *aux = board[pawn->piece_location];
                piece_entry * aux2 = board[test_move];
                board[test_move] = aux;
                board[pawn->piece_location] = NULL;

                /* check if king is safe after the move */
                if (check_chess(king->piece_location, color)) {
                /* the king is safe after the piece is taken */
                    (pawn->possible_moves).push_back(test_move);
                    number_of_moves ++;
                }

                /* restore board to originalstate */
                board[pawn->piece_location] = aux;
                board[test_move] = aux2;
        }
    }

    return number_of_moves;
}