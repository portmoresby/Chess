#pragma once
#include "pieces.h"

using namespace std;

class Player {
public:
	string name;
	Color color;
	bool turn = false;
	pair<int, int> kingLoc; // Location of the king for this player
	vector<pair<int, int>> oppoVision;
	bool inCheck = false;
	Player(string n, Color c) {
		name = n;
		color = c;
		if (c == WHITE) {
			turn = true;
			kingLoc = make_pair(0, 4); // White king starts at e1
		}
		if (c == BLACK)
		{
			kingLoc = make_pair(7, 4);
		}
	}
	Player() : name(""), color(NULL_COLOR) {}
};

class Game {
public:
	vector<vector<Piece>> board;
	Player p1;
	Player p2;
	void initializeBoard() {
		char files[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		for (int r = 1; r <= 8; r++) {
			vector<Piece> row;
			for (char f : files) {
				if (r == 1)
				{
					switch (f) {
					case 'a': row.push_back(Piece(ROOK, WHITE, 'a', 1)); break;
					case 'b': row.push_back(Piece(KNIGHT, WHITE, 'b', 1)); break;
					case 'c': row.push_back(Bishop(WHITE, 'c', 1)); break;
					case 'd': row.push_back(Piece(QUEEN, WHITE, 'd', 1)); break;
					case 'e': row.push_back(Piece(KING, WHITE, 'e', 1)); break;
					case 'f': row.push_back(Bishop(WHITE, 'f', 1)); break;
					case 'g': row.push_back(Piece(KNIGHT, WHITE, 'g', 1)); break;
					case 'h': row.push_back(Rook(WHITE, 'h', 1)); break;
					}
				}
				else if (r == 2)
				{
					row.push_back(Pawn(WHITE, f, r));
				}
				else if (r == 7)
				{
					row.push_back(Pawn(BLACK, f, r));
				}
				else if (r == 8)
				{
					switch (f) {
					case 'a': row.push_back(Piece(ROOK, BLACK, 'a', 8)); break;
					case 'b': row.push_back(Piece(KNIGHT, BLACK, 'b', 8)); break;
					case 'c': row.push_back(Piece(BISHOP, BLACK, 'c', 8)); break;
					case 'd': row.push_back(Piece(QUEEN, BLACK, 'd', 8)); break;
					case 'e': row.push_back(Piece(KING, BLACK, 'e', 8)); break;
					case 'f': row.push_back(Piece(BISHOP, BLACK, 'f', 8)); break;
					case 'g': row.push_back(Piece(KNIGHT, BLACK, 'g', 8)); break;
					case 'h': row.push_back(Piece(ROOK, BLACK, 'h', 8)); break;
					}
				}
				else
				{
					row.push_back(Piece(EMPTY, NULL_COLOR, f, r));
				}
			}
			board.push_back(row);
		}
	}

	Game(Player player1, Player player2) {
		player1.color = WHITE;
		player2.color = BLACK;
		p1 = player1;
		p2 = player2;
		initializeBoard();
	}
	PieceType chartoPieceType(char c) {
		switch (c) {
		case 'R': return ROOK;
		case 'N': return KNIGHT;
		case 'B': return BISHOP;
		case 'Q': return QUEEN;
		case 'K': return KING;
		default: return PAWN;
		}
	}
	void boardVision(Color c);
	void conductCastle(Color c, string longshort);

	vector<pair<int, int>> oppositionVision(Color c);
	bool checkChecker(Color oppo);
	vector<vector<int>> legalMovesinCheck(Color c, vector<pair<int, int>> oppoVision);
	pair<int, int> findChecker();

	vector<int> findMovingPawn(Color c, pair<int, int> endloc, int correct);
	vector<int> findMovingBishop(Color c, pair<int, int> endloc);
	vector<int> findMovingKnight(Color c, pair<int, int> endloc);
	vector<int> findMovingRook(Color c, pair<int, int> endloc);
	vector<int> findMovingQueen(Color c, pair<int, int> endloc);
	vector<int> findMovingKing(Color c, pair<int, int> endloc);

	vector<int> notationToMove(Color color, string notation, bool inCheck, vector<vector<int>> legalMoves);
	void movePiece(vector<int> move);
};

void Game::boardVision(Color c) {
	cout << endl;
	if (c == WHITE)
	{
		cout << "    --      White POV     --" << endl;
		cout << "    ========================" << endl;
		for (int i = 7; i >= 0; i--)
		{
			int rank = board[i][0].location.second;
			cout << rank << "  |";
			for (int j = 0; j < 8; j++)
			{
				bool light = isLightSquare(i + j);
				c = board[i][j].color; // See the color for the current Piece
				if (light == true)
				{
					//cout << "\33[47m";
				}
				if (c == BLACK)
				{
					cout << "\33[1;30m";
				}
				if (c == WHITE)
				{
					cout << "\33[97m";
				}
				cout << board[i][j].toStringBoard();
				cout << "\33[0m" << "|"; // Reset color
			}
			cout << endl;
			if (i % 2 == 0 && i != 0)
			{
				cout << "    ===---===---===---===---" << endl;
			}
			else if (i % 2 == 1)
			{
				cout << "    ---===---===---===---===" << endl;
			}
		}
		cout << "    ========================" << endl;
		cout << "     a  b  c  d  e  f  g  h " << endl;
	}
	if (c == BLACK)
	{
		cout << "   --      Black POV      --" << endl;
		cout << "   =========================" << endl;
		for (int i = 0; i < 8; i++)
		{
			int rank = board[i][0].location.second;
			cout << rank << "  |";
			for (int j = 7; j >= 0; j--)
			{
				bool light = isLightSquare(i + j);
				c = board[i][j].color; // See the color for the current Piece
				if (light == true)
				{
					// cout << "\33[47m";
				}
				if (c == BLACK)
				{
					cout << "\33[90m";
				}
				if (c == WHITE)
				{
					cout << "\33[97m";
				}
				cout << board[i][j].toStringBoard();
				cout << "\33[0m" << "|"; // Reset color
			}
			cout << endl;
			if (i % 2 == 0)
			{
				cout << "    ===---===---===---===---" << endl;
			}
			else if (i % 2 == 1 && i != 7)
			{
				cout << "    ---===---===---===---===" << endl;
			}
		}
		cout << "    ========================" << endl;
		cout << "     h  g  f  e  d  c  b  a  " << endl;
	}
}

void Game::conductCastle(Color c, string longshort) {
	if (longshort == "O-O-O")
	{
		if (c == BLACK)
		{
			board[7][2] = board[7][4]; // Move the king to c1
			board[7][4] = Piece(EMPTY, NULL_COLOR, 'e', 1); // Remove the king from e1
			board[7][3] = board[7][0]; // Move the rook to d1
			board[7][0] = Piece(EMPTY, NULL_COLOR, 'a', 1); // Remove the rook from a1
		}
		else if (c == WHITE)
		{
			board[0][2] = board[0][4]; // Move the king to c8
			board[0][4] = Piece(EMPTY, NULL_COLOR, 'e', 8); // Remove the king from e8
			board[0][3] = board[0][0]; // Move the rook to d8
			board[0][0] = Piece(EMPTY, NULL_COLOR, 'a', 8); // Remove the rook from a8
		}
	}
	else if (longshort == "O-O")
	{
		if (c == BLACK)
		{
			board[7][6] = board[7][4]; // Move the king to g1
			board[7][4] = Piece(EMPTY, NULL_COLOR, 'e', 1); // Remove the king from e1
			board[7][5] = board[7][7]; // Move the rook to f1
			board[7][7] = Piece(EMPTY, NULL_COLOR, 'h', 1); // Remove the rook from h1
		}
		else if (c == WHITE)
		{
			board[0][6] = board[0][4]; // Move the king to g8
			board[0][4] = Piece(EMPTY, NULL_COLOR, 'e', 8); // Remove the king from e8
			board[0][5] = board[0][7]; // Move the rook to f8
			board[0][7] = Piece(EMPTY, NULL_COLOR, 'h', 8); // Remove the rook from h8
		}
	}
	return;
}

vector<pair<int, int>> Game::oppositionVision(Color oppo) {
	vector<pair<int, int>> vision;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece p = board[i][j];
			if (p.color == oppo)
			{
				if (p.type == PAWN)
				{
					Pawn P;
					P.location = board[i][j].location;
					P.color = board[i][j].color;
					P.hasMoved = board[i][j].hasMoved;
					P.pieceVision(board, false);
					for (auto m : P.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
					continue;
				}
				if (p.type == KNIGHT)
				{
					Knight N;
					N.location = board[i][j].location;
					N.color = board[i][j].color;
					N.hasMoved = board[i][j].hasMoved;
					N.pieceVision(board, false);
					for (auto m : N.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
					continue;
				}
				if (p.type == BISHOP)
				{
					Bishop B;
					B.location = board[i][j].location;
					B.color = board[i][j].color;
					B.hasMoved = board[i][j].hasMoved;
					B.pieceVision(board, false);
					for (auto m : B.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
					continue;
				}
				if (p.type == ROOK)
				{
					Rook R;
					R.location = board[i][j].location;
					R.color = board[i][j].color;
					R.hasMoved = board[i][j].hasMoved;
					R.pieceVision(board, false);
					for (auto m : R.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
					continue;
				}
				if (p.type == QUEEN)
				{
					Queen Q;
					Q.location = board[i][j].location;
					Q.color = board[i][j].color;
					Q.hasMoved = board[i][j].hasMoved;
					Q.pieceVision(board, false);
					for (auto m : Q.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
					continue;
				}
				if (p.type == KING)
				{
					King K;
					K.location = board[i][j].location;
					K.color = board[i][j].color;
					K.hasMoved = board[i][j].hasMoved;
					K.pieceVision(board, false, (oppo == WHITE) ? p1.oppoVision : p2.oppoVision);
					for (auto m : K.possibleMoves)
					{
						if (find(vision.begin(), vision.end(), m) != vision.end())
						{
							continue;
						}
						vision.push_back(m);
					}
				}
				continue;
			}
		}
	}
	return vision;
}

bool Game::checkChecker(Color oppo) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].givingCheck == true && board[i][j].color == oppo)
			{
				return true;
			}
		}
	}
	return false; // No checker found
}

pair<int, int> Game::findChecker() {
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			if (board[i][j].givingCheck == true)
			{
				return make_pair(i, j);
			}
		}
	}
}

vector<vector<int>> Game::legalMovesinCheck(Color c, vector<pair<int, int>> oppoVision) {
	vector <vector<int>> legalMoves;
	vector<pair<int, int>> squaresTo;
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			if (board[i][j].type == KING && board[i][j].color == c)
			{
				King temp;
				temp.color = board[i][j].color;
				temp.location = board[i][j].location;
				temp.pieceVision(board, true, oppoVision);
				for (size_t k = 0; k < temp.possibleMoves.size(); k++)
				{
					vector<int> m;
					m.push_back(i); // From row
					m.push_back(j); // From column
					m.push_back(temp.possibleMoves[k].first); // To row
					m.push_back(temp.possibleMoves[k].second); // To column
					legalMoves.push_back(m); // Add the move to legalMoves
				}
			}
			if (board[i][j].givingCheck == true)
			{
				squaresTo.push_back(make_pair(i, j)); // The square of the checking piece
				for (auto k : board[i][j].pathForCheck)
				{
					squaresTo.push_back(k);
				}
				board[i][j].pathForCheck.clear(); // Clear the path for check after using it
			}
		}
	}
	// Now find all pieces of color c that can move to any square in squaresTo
	for (auto square : squaresTo)
	{
		vector<int> move;
		move = findMovingPawn(c, square, 20);
		if (!move.empty())
		{
			legalMoves.push_back(move);
		}
		move = findMovingBishop(c, square);
		if (!move.empty())
		{
			legalMoves.push_back(move);
		}
		move = findMovingKnight(c, square);
		if (!move.empty())
		{
			legalMoves.push_back(move);
		}
		move = findMovingRook(c, square);
		if (!move.empty())
		{
			legalMoves.push_back(move);
		}
		move = findMovingQueen(c, square);
		if (!move.empty())
		{
			legalMoves.push_back(move);
		}
	}
	return legalMoves;
}

vector<int> Game::findMovingPawn(Color c, pair<int, int> endloc, int correct) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == PAWN && board[i][j].color == c)
			{
				Pawn temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						if (correct != 20 && correct != j)
						{
							break;
						}
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching pawn is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving pawn found
}

vector<int> Game::findMovingBishop(Color c, pair<int, int> endloc) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == BISHOP && board[i][j].color == c)
			{
				Bishop temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching bishop is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving bishop found
}

vector<int> Game::findMovingKnight(Color c, pair<int, int> endloc) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == KNIGHT && board[i][j].color == c)
			{
				Knight temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching knight is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving knight found
}

vector<int> Game::findMovingRook(Color c, pair<int, int> endloc) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == ROOK && board[i][j].color == c)
			{
				Rook temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching rook is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving rook found
}

vector<int> Game::findMovingQueen(Color c, pair<int, int> endloc) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == QUEEN && board[i][j].color == c)
			{
				Queen temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching queen is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving queen found
}

vector<int> Game::findMovingKing(Color c, pair<int, int> endloc) {
	vector<int> move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].type == KING && board[i][j].color == c)
			{
				King temp;
				temp.location = board[i][j].location;
				temp.color = board[i][j].color;
				temp.hasMoved = board[i][j].hasMoved;
				temp.pieceVision(board, true, (c == WHITE) ? p1.oppoVision : p2.oppoVision);
				for (auto k : temp.possibleMoves)
				{
					if (k.first == endloc.first && k.second == endloc.second)
					{
						move.push_back(i); // From row
						move.push_back(j); // From column
						move.push_back(endloc.first); // To row
						move.push_back(endloc.second); // To column
						return move; // Return the move if a matching king is found
					}
				}
			}
		}
	}
	return move; // Return empty vector if no moving king found
}

vector<int> Game::notationToMove(Color c, string notation, bool inCheck, vector<vector<int>> legalMoves) {
	if (inCheck)
	{
		if (legalMoves.size() == 0)
		{
			cout << "Checkmate! No legal moves available." << endl;
			return {};
		}
		for (auto m : legalMoves)
		{
			break; //turn off to print all legal moves
			cout << coordToString(make_pair(m[0], m[1])) << " to " << coordToString(make_pair(m[2], m[3])) << endl;
		}
	}
	if (notation == "O-O" || notation == "O-O-O")
	{
		conductCastle(c, notation);
		vector<int> curr = { 5 };
		return curr; // Return one element vector for castling
	}
	vector<int> move;
	int correct = 20;
	Piece mover;
	int s = notation.size();
	if (s == 2) mover.type = PAWN;
	else if (s >= 3)
	{
		mover.type = chartoPieceType(notation[0]);
	}
	string endcoord = notation.substr(notation.length() - 2, 2);
	pair<int, int> endloc = rftcd(parLoc(endcoord));
	switch (mover.type)
	{
	case PAWN: 
		if (notation.size() > 2)
		{
			correct = notation[0] - 'a';
		}
		move = findMovingPawn(c, endloc, correct);
		if (move.size() == 0) {
			cout << "No pawns can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5);
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
	case BISHOP:
		move = findMovingBishop(c, endloc);
		if (move.size() == 0)
		{
			cout << "No bishops can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5); 
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
		break;
	case KNIGHT:
		move = findMovingKnight(c, endloc);
		if (move.size() == 0)
		{
			cout << "No knights can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5);
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
		break;
	case ROOK:
		move = findMovingRook(c, endloc);
		if (move.size() == 0)
		{
			cout << "No rooks can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5);
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
		break;
	case QUEEN:
		move = findMovingQueen(c, endloc);
		if (move.size() == 0)
		{
			cout << "No queens can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5);
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
		break;
	case KING:
		move = findMovingKing(c, endloc);
		if (move.size() == 0)
		{
			cout << "No kings can move there." << endl;
		}
		if (inCheck == true)
		{
			if (find(legalMoves.begin(), legalMoves.end(), move) == legalMoves.end())
			{
				cout << "That's an illegal move, as you're still in check! try again." << endl;
				move.clear();
				move.push_back(5);
				move.push_back(6);
			}
			pair<int, int> checker = findChecker();
			board[checker.first][checker.second].givingCheck = false;
		}
		return move;
		break;
	default:
		break;
	}
	return move;
}

void Game::movePiece(vector<int> move) {
	if (move.size() == 1)
	{
		return;
	}
	pair<int, int> fromLoc(move[0], move[1]);
	pair<int, int> toLoc(move[2], move[3]);
	if (fromLoc.first < 0 || fromLoc.first >= 8 || fromLoc.second < 0 || fromLoc.second >= 8 ||
		toLoc.first < 0 || toLoc.first >= 8 || toLoc.second < 0 || toLoc.second >= 8) {
		cout << "Invalid move: out of bounds." << endl;
		return;
	}
	if (board[fromLoc.first][fromLoc.second].type == PAWN && (toLoc.first == 0 || toLoc.first == 7))
	{
		cout << "Pawn promotion! Choose a piece to promote to (Q, R, B, N): ";
		string promotionChoice;
		cin >> promotionChoice;
		switch (promotionChoice[0])
		{
		case 'Q':
			board[toLoc.first][toLoc.second] = Piece(QUEEN, board[fromLoc.first][fromLoc.second].color, toLoc.second + 'a', toLoc.first);
			board[toLoc.first][toLoc.second].hasMoved = true; // Mark the piece as moved
			break;
		case 'R':
			board[toLoc.first][toLoc.second] = Piece(ROOK, board[fromLoc.first][fromLoc.second].color, toLoc.second + 'a', toLoc.first);
			board[toLoc.first][toLoc.second].hasMoved = true; // Mark the piece as moved
			break;
		case 'B':
			board[toLoc.first][toLoc.second] = Bishop(board[fromLoc.first][fromLoc.second].color, toLoc.second + 'a', toLoc.first);
			board[toLoc.first][toLoc.second].hasMoved = true; // Mark the piece as moved
			break;
		case 'N':
			board[toLoc.first][toLoc.second] = Knight(board[fromLoc.first][fromLoc.second].color, toLoc.second + 'a', toLoc.first);
			board[toLoc.first][toLoc.second].hasMoved = true; // Mark the piece as moved
			break;

		default:
			break;
		}
	}
	else {
		board[toLoc.first][toLoc.second] = board[fromLoc.first][fromLoc.second];
		board[toLoc.first][toLoc.second].hasMoved = true; // Mark the piece as moved
	}
	board[fromLoc.first][fromLoc.second] = Piece(EMPTY, NULL_COLOR, fromLoc.second + 'a', fromLoc.first); // Clear previous position
	return;
}

