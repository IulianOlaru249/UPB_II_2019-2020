********************************************************************************
How to play
********************************************************************************

Build the engine:
make

Make sure you have xBoard installed. If not:
sudo apt-get install xboard

Run the game with the engine:
make run

Run the game in debug mode (there will be created an xboard.debug file):
make debug

Clean the log files and the executable:
make clean

********************************************************************************
Team
********************************************************************************

This project was a team effort and the challenges were tackled by all members.

Most of the communication with xBoard and response to commands was implemented
by Nicusor Slabu.

Most of the code structure managing and piece representation on board was done 
byIulian Olaru.

Most of the move generating logic and some key board features were implemented 
by Adrian Filimon.

The board representation was mostly implemented as a team effort.

********************************************************************************
Board representation
********************************************************************************

	In order to store the board in memory, we use an array of 128 elements. Each 
index that satisfies the predicat (index and 0x88 is 0) is a valid square. 
Other squares are not considered part of the board. On each square is stored 
the address of a piece. We store addresses and not the piece itself because it
is faster to get a pointer from memory (which has a fix length) than a 
structure.

/*NOTE: The board is printed in the log file after each move, so it cat be 
checked by inspecting the log file */

********************************************************************************
Piece representation
********************************************************************************

	The pieces are structures that can be found in pieces.h. Each has an unique
value (by which we decide its type. Eg -1 :black pawn, 1: white pawn), an array 
of possible moves, a field to decide if the piece has been captured or not 
and two fields: the current position and the next position ( which will be 
picked from the possible moves). Those last fields will help us update the
board.

	The pieces are separated from the board and stored in arrays in order to 
limit the number of iteration necesarry to access each piece. We will have a 
maximum of 32 iteration to access each piece, as contrary to 128 if the pieces 
were directly stored on the board.

********************************************************************************
xBoard communication
********************************************************************************

	For now, or engine supports the following xBoard commands:

 		-protover: where we tell xBoard what are the features of our engine	

		-new: set the engine to play as black (default), initialize the arrays 
			of pieces and place them on the board. It is called when we start 
			a new game.

		-force: the engine goes into analysis mode and plays neither color, only
			updates the board represented in memory based on user moves. *this 
			can be checked by viewing the log file*

		-black/white: the engine will now play for the opposite color. This can
			be set any time during the game. :)

		-go: the engine will make a move imidiately, for the color in play. 
			It is often called after a change of color, and it makes sure the 
			engine still makes a move with the new color.

		-resign: when the engine runs out of possible moves, it resigns.

		-quit: engine resigns, the process is killed and the game ends.
		
		-usermove: we update the board with the player's move and log it.


********************************************************************************
How we make moves
********************************************************************************

	For now, we make random valid moves for the knights (as a demo, the engine 
plays only with the knight from left). The process is documented in the header  
move_gen.h.

	/*NOTE: the engine DOES NOT account for CHECK yet. When its king is in check
the engine will still try to make a move with the horse and it will be invalid.
A new game can always be started. */

********************************************************************************
Sources
********************************************************************************

https://github.com/Jetersen/chess
https://www.gnu.org/software/xboard/engine-intf.html#7
https://github.com/Mk-Chan/WyldChess/blob/master/src/
http://www.craftychess.com/hyatt/boardrep.html
https://www.gnu.org/software/chess/
