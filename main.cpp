#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <random>
#include <ctime>
#include <windows.h> 
#include "game.h"

using namespace std;

int main() {
	Player p1("White", WHITE);
	Player p2("Black", BLACK);
	Game game(p1, p2);
	cout << "Welcome to Chess!" << endl;
	int move = 1; // 0 for white, 1 for black
	bool gameOver = false;
	while (gameOver == false) {
		cout << endl << endl << "Move " << move << ": ";
		cout << "White's turn. If you would like to resign, enter 'resign'. Else, enter your move." << endl;
		game.boardVision(WHITE);
		bool wCastle = false;
		string moveW;
		cin >> moveW;
		if (moveW == "resign")
		{
			gameOver = true;
			cout << "White has resigned. Black wins!" << endl;
			break;
		}
		vector<int> moveCoords = game.notationToMove(WHITE, moveW);
		if (moveCoords.size() != 4 && moveCoords.size() != 1)
		{
			cout << "Invalid move. Please try again." << endl;
			continue;
		}
		game.movePiece(moveCoords);
		moveCoords.clear();
		
		cout << "White moved." << endl << endl << endl;
		cout << "Black's turn. If you would like to resign, enter 'resign'. Else, enter your move." << endl;
		game.boardVision(BLACK);
		bool bCastle = false;
		string moveB;
		cin >> moveB;
		if (moveB == "resign")
		{
			gameOver = true;
			cout << "Black has resigned. White wins!" << endl;
			break;
		}
		moveCoords = game.notationToMove(BLACK, moveB);
		if (moveCoords.size() != 4 && moveCoords.size() != 1)
		{
			cout << "Invalid move. Please try again." << endl;
			continue;
		}
		cout << "Black moved." << endl;
		game.movePiece(moveCoords);
		moveCoords.clear();
		move++;
	}
	return 0;
}



/*
TO-DO:
DO I NEED A MOVE CLASS?
- Current Weird Behavior:
	- Bishop moved to wrong square gives error instead of second chance
	- 
- Add check and checkmate detection
	- opposition TOTAL piece vision vector
	- checkPath vector compared with personal piece vision vector
	- see if checker is capturable
- Add pawn promotion
	- If pawn reaches the end of the board, prompt for promotion
	- Change pawn type to queen, rook, bishop, or knight
- Add en passant
	- justMovedPawn2Squares checker
	- check adjacency of pawn to justMovedPawn2Squares
-Add piece delineation
	- Pawns done
	- Knights, Bishops, and Queens need rank, file, and both coord checkers
		- will either be only location[1] or will include location[2]
	- Rooks need rank or file checkers
		- will always be location[1]
- Add draw detection
	- Stalemate: SEE BELOW
	- 50 moves without a pawn move or capture
	- If insufficient material 
		- Check for all combinations of pieces that can lead to a draw
	- If threefold repetition
		- Keep track of board states and compare them
- Add castling checks
	- Check if rook or king has moved
	- Check if squares between king and rook are empty
	- piece vision for opposition pieces != king's path
-Add pin detection
	- Check if piece is pinned by opposition piece
	- For everything but Knight, Check if piece can move without leaving king in check
		- if so, in which direction can it move?
	- Limit vision of pinned piece to only the direction it can move
- Add stalemate detection
	- If vision of all personal pieces is empty and king is not in check, it's a stalemate
- General QOL
	- option to see possible moves for a given piece
	- Mess around with board colors / maybe provide options
	- Add notation for any given game
		- prompt for notation before each move
*/