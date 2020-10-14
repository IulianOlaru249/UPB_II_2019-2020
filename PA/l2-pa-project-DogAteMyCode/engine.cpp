#include "xBoard.h" //for cumunicating with xBoard
#include "board_0x88.h" //to represent the board and pieces in memory
#include "move_gen.h" //for generaing moves for the pieces

/*NOTE: all response to command are in the if statement and not in functions
 * in order to limit the calls and to kepp the stack frame small
 */

int main() {

    srand (time(NULL));
    int random_piece;

	char *opponent_move;
	char input[STR_BUFF];

	int pawn_moves;
	int rook_moves;
	int knight_moves;
	int bishop_moves;
	int king_moves;
	int queen_moves;


	while (true) {
		fflush(stdout);

		if (!fgets(input, STR_BUFF, stdin)){
            printf("Exit");
            return 0;
        }

        if(!strcmp(input, "xboard\n")) {
			printf("\n");
        }

		else if (!strcmp(input, "protover 2\n")) {
			printf("feature done=0\n"); /* tell board to expect a list of features */
			printf("feature ping=1\n");  
			printf("feature myname=\"%s\"\n", ENGINE_NAME); /* set engine name */
			printf("feature reuse=0\n"); /*start fresh game */
			printf("feature sigint=0\n");  /* disable signals */
			printf("feature usermove=1\n");
			printf("feature done=1\n"); /* tell board list of features is done */
		}

		else if (!strcmp(input, "new\n")) {
			/* set engine to play as black */
			engine_side	= Black;

			/* initialize pieces */
			init_black_pieces();
			init_white_pieces();

			/* set pieces on board */
			set_board();
		}

		else if(!strcmp(input, "force\n")) {
			/* set engine to play as neither so it dosen't make moves */
			engine_side = Neither;
			//print_board();
		}

		else if(!strcmp(input, "white\n")) {
			/* set engine to play as white */
			engine_side = White;
		}

		else if(!strcmp(input, "black\n")) {
			/* set engine to play as black */
			engine_side = Black;
		}

		else if(!strcmp(input, "go\n")) {
			/* go to the section that updates the board */
			goto make_move;
		}

		/* engine expects a pong command */
		else if(!strncmp(input, "ping", 4)) {
			printf("pong %d\n", atoi(input + 5));
		}

		/* when the user clicks on resign */
        if(!strncmp(input, "result", 6)) {
			/*engine resigns as well*/
			printf("resign\n");
        }

        if(!strncmp(input, "quit", 4)) {
			/* Make sure the engine resignes and the process is killed */
			printf("resign\n");
			exit(0);
        }

		else {
			if (!strncmp(input, "usermove", 8)) {
				opponent_move = input + 9;
			}
			else if (input[1] < '9' && input[1] > '0') {
				opponent_move = input;
			}
			else {
				continue;
			}

			/* update the board with the move of the opponent */
			update_board (opponent_move);

make_move:
			
			/* Generate moves for black and white pawns */
			for (int i = 0; i < 8; i++) {
				if (black_piece_entries[i].piece_value == -1)
					pawn_moves = generate_moves_pawn(&black_piece_entries[i]);
				else 
					pawn_moves = generate_moves_queen(&black_piece_entries[i]);
			}
			for (int i = 0; i < 8; i++) {
				if (white_piece_entries[i].piece_value == 1)
					pawn_moves = generate_moves_pawn(&white_piece_entries[i]);
				else
					pawn_moves = generate_moves_queen(&white_piece_entries[i]);
			}


			/* Generate moves for black and white rook */
			rook_moves = generate_moves_rook(&black_piece_entries[8]);
			rook_moves = generate_moves_rook(&black_piece_entries[15]);

			rook_moves = generate_moves_rook(&white_piece_entries[8]);
			rook_moves = generate_moves_rook(&white_piece_entries[15]);

			/* Generate moves for black and white knight */
			knight_moves = generate_moves_knight(&black_piece_entries[9]);
			knight_moves = generate_moves_knight(&black_piece_entries[14]);

			knight_moves = generate_moves_knight(&white_piece_entries[9]);
			knight_moves = generate_moves_knight(&white_piece_entries[14]);

			/* Generate moves for black and white bishop */
			bishop_moves = generate_moves_bishop(&black_piece_entries[10]);
			bishop_moves = generate_moves_bishop(&black_piece_entries[13]);

			bishop_moves = generate_moves_bishop(&white_piece_entries[10]);
			bishop_moves = generate_moves_bishop(&white_piece_entries[13]);

			/* Generate moves for black and white queen */
			queen_moves = generate_moves_queen(&black_piece_entries[11]);
			queen_moves = generate_moves_queen(&white_piece_entries[11]);

			/* Generate moves for black and white king */
			king_moves = generate_moves_king(&black_piece_entries[12]);
			king_moves = generate_moves_king(&white_piece_entries[12]);

			if(engine_side == Black)
			{
				do {
					random_piece = rand() % 16;
					if (black_piece_entries[random_piece].possible_moves.size() != 0)
						break;
				} while (true);

				char *current_move = (char *) malloc (6 * sizeof (char));
				int random_pos = rand () % black_piece_entries[random_piece].possible_moves.size();
				//int random_pos = 0;
				get_square_code (current_move, black_piece_entries[random_piece].piece_location, 
								 black_piece_entries[random_piece].possible_moves[random_pos]);


				if (black_piece_entries[random_piece].piece_value == -1) {
					if (black_piece_entries[random_piece].possible_moves[random_pos] >= 0 &&
						black_piece_entries[random_piece].possible_moves[random_pos] <= 7){
							current_move[4] = 'q';
							current_move[5] = '\0';
						}
				}

				printf("move %s\n", current_move);
				update_board(current_move);
			}
			else if (engine_side == White)
			{
				do {
					random_piece = rand() % 16;
					if (white_piece_entries[random_piece].possible_moves.size() != 0)
						break;
				} while (true);

				char *current_move = (char *) malloc (6 * sizeof (char));
				int random_pos = rand () % white_piece_entries[random_piece].possible_moves.size();
				get_square_code (current_move, white_piece_entries[random_piece].piece_location, 
								 white_piece_entries[random_piece].possible_moves[random_pos]);

				if (white_piece_entries[random_piece].piece_value == 1) {
					if (white_piece_entries[random_piece].possible_moves[random_pos] >= 112 &&
						white_piece_entries[random_piece].possible_moves[random_pos] <= 119){
							current_move[4] = 'q';
							current_move[5] = '\0';
						}
				}

				printf("move %s\n", current_move);
				update_board(current_move);
			}

			/* log the board and all moves */
			//print_board();
			// for (int i = 0; i < 16; i++) {
			// 	log_moves(&black_piece_entries[i]);
			// }

			// for (int i = 0; i < 16; i++) {
			// 	log_moves(&white_piece_entries[i]);
			// }

			//log.open("log.txt",std::ios_base::app);
			//log <<"Random piece!!!!!!"<< black_piece_entries[random_piece].piece_value << endl;
			//log.close();
		}
	}

	return 0;
}
