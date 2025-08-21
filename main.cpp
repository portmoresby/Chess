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
	// Initialize the random number generator
	Player p1("White", WHITE);
	Player p2("Black", BLACK);
	Game game(p1, p2);
	cout << "Welcome to Chess!" << endl;
	int move = 1; // 0 for white, 1 for black
	bool gameOver = false;
	while (gameOver == false) {
		vector<int> moveCoords;
		if (p1.turn)
		{
			cout << "Move " << move << ": ";
			bool wCastle = false;
			game.boardVision(WHITE);
			p1.oppoVision.clear();
			p1.oppoVision = game.oppositionVision(BLACK);
			p1.inCheck = game.checkChecker(BLACK);
			if (p1.inCheck)
			{
				vector<vector<int>> mate = game.legalMovesinCheck(WHITE, p1.oppoVision);
				if (mate.size() == 0)
				{
					cout << "White is checkmated! Game Over." << endl;
					gameOver = true;
					break;
				}
				cout << "White is in check!" << endl;
			}
			cout << "White's turn. If you would like to resign, enter 'resign'. Else, enter your move." << endl;
			string moveW;
			cin >> moveW;
			if (moveW == "resign")
			{
				gameOver = true;
				cout << "White has resigned. Black wins!" << endl;
				break;
			}
			moveCoords = game.notationToMove(WHITE, moveW, p1.inCheck, p1.oppoVision);
			if (moveCoords.size() != 4 && moveCoords.size() != 1)
			{
				cout << "Invalid move. Please try again." << endl;
				continue;
			}
			game.movePiece(moveCoords);
			p2.turn = true;
			p1.turn = false;
			moveCoords.clear();
			cout << "White moved." << endl << endl << endl;
		}
		if (p2.turn) {
			bool bCastle = false;
			game.boardVision(BLACK);
			p2.oppoVision.clear();
			p2.oppoVision = game.oppositionVision(WHITE);
			p2.inCheck = game.checkChecker(WHITE);
			if (p2.inCheck)
			{
				vector<vector<int>> mate = game.legalMovesinCheck(BLACK, p2.oppoVision);
				if (mate.size() == 0)
				{
					cout << "Black is checkmated! Game Over." << endl;
					gameOver = true;
					break;
				}
				cout << "Black is in check!" << endl;
			}
			cout << "Black's turn. If you would like to resign, enter 'resign'. Else, enter your move." << endl;
			string moveB;
			cin >> moveB;
			if (moveB == "resign")
			{
				gameOver = true;
				cout << "Black has resigned. White wins!" << endl;
				break;
			}
			moveCoords = game.notationToMove(BLACK, moveB, p2.inCheck, p2.oppoVision);
			if (moveCoords.size() != 4 && moveCoords.size() != 1)
			{
				cout << "Invalid move. Please try again." << endl;
				continue;
			}
			game.movePiece(moveCoords);
			p1.turn = true;
			p2.turn = false;
			moveCoords.clear();
			cout << "Black moved." << endl << endl << endl;
			move++;
		}
	}
	return 0;
}



/*
TO-DO:
DO I NEED A MOVE CLASS?


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



