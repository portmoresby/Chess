#pragma once

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

using namespace std;




string coordToString(pair<int, int> loc) {
	return to_string(loc.first) + to_string(loc.second);
}

pair<char, int> parLoc(string loc) {
	if (loc.length() != 2) return make_pair('x', 0); // Invalid location
	char file = tolower(loc[0]);
	int rank = loc[1] - '0';
	if (file < 'a' || file > 'h' || rank < 1 || rank > 8) return make_pair('x', 0); // Invalid location
	return make_pair(file, rank);
}

pair<int, int> rftcd(pair<char, int> loc) {
	return make_pair(loc.second - 1, loc.first - 'a');
}




enum Color {
	NULL_COLOR,
	WHITE,
	BLACK
};

enum PieceType {
	EMPTY,
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

class Piece {
public:
	PieceType type;
	Color color;
	pair<char, int> location; // file, rank
	vector<pair<int, int>> possibleMoves;
	bool hasMoved = false; // Used for pawns to determine if they can move two squares forward
	Piece(PieceType t, Color c, char f, int r) {
		type = t;
		color = c;
		location = make_pair(f, r);
	}
	Piece() : type(EMPTY), color(NULL_COLOR), location(make_pair('a', 1)) {}
	string toString();
	string locString() { return string(1, location.first) + to_string(location.second); }
	string toStringBoard();
	virtual void pieceVision(vector<vector<Piece>> board) {}
	Piece& operator=(const Piece& other) {
		type = other.type;
		color = other.color;
		hasMoved = true;
		return *this;
	}

};

string Piece::toString() {
	switch (type) {
		case EMPTY: return "Empty";
		case PAWN: return "Pawn";
		case ROOK: return "Rook";
		case KNIGHT: return "Knight";
		case BISHOP: return "Bishop";
		case QUEEN: return "Queen";
		case KING: return "King";
		default: return "Unknown";
	}
}

string Piece::toStringBoard() {
	if (color == WHITE)
	{
		switch (type) {
			case EMPTY: return "  ";
			case PAWN: return "P ";
			case KNIGHT: return "N ";
			case BISHOP: return "B ";
			case ROOK: return "R ";
			case QUEEN: return "Q ";
			case KING: return "K ";
			default: return "* ";
		}
	}
	if (color == BLACK)
	{
		switch (type) {
		case EMPTY: return "  ";
		case PAWN: return "p ";
		case KNIGHT: return "n ";
		case BISHOP: return "b ";
		case ROOK: return "r ";
		case QUEEN: return "q ";
		case KING: return "k ";
		default: return "* ";
		}
	}
	return "  ";
}

class Pawn : public Piece {
public:
	bool promoted = false;
	bool captured = false;
	bool givingCheck = false;
	
	Pawn(Color c, char f, int r) : Piece(PAWN, c, f, r) {}
	Pawn() : Piece(PAWN, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void Pawn::pieceVision(vector<vector<Piece>> board) {
	pair<int, int> loc = rftcd(location);
	if (color == WHITE)
	{
		if (board[loc.first + 1][loc.second].type == EMPTY)
		{
			possibleMoves.push_back(make_pair(loc.first + 1, loc.second));
			if (hasMoved == false)
			{
				if (board[loc.first + 2][loc.second].type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + 2, loc.second));
				}
			}
		}
		if (loc.second == 0)
		{
			if (board[loc.first + 1][loc.second + 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(loc.first + 1, loc.second + 1));
			}
		}
		if (loc.second == 7)
		{
			if (board[loc.first + 1][loc.second - 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(loc.first + 1, loc.second - 1));
			}
		}
		if (loc.second > 0 && loc.second < 7)
		{
			if (board[loc.first + 1][loc.second + 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(loc.first + 1, loc.second + 1));
			}
			if (board[loc.first + 1][loc.second - 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(loc.first + 1, loc.second - 1));
			}
		}
	}
	if (color == BLACK)
	{
		if (board[loc.first - 1][loc.second].type == EMPTY)
		{
			possibleMoves.push_back(make_pair(loc.first - 1, loc.second));
			if (hasMoved == false)
			{
				if (board[loc.first - 2][loc.second].type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - 2, loc.second));
				}
			}
		}
		if (loc.second == 0)
		{
			if (board[loc.first - 1][loc.second + 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(loc.first - 1, loc.second + 1));
			}
		}
		if (loc.second == 7)
		{
			if (board[loc.first - 1][loc.second - 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(loc.first - 1, loc.second - 1));
			}
		}
		if (loc.second > 0 && loc.second < 7)
		{
			if (board[loc.first - 1][loc.second + 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(loc.first - 1, loc.second + 1));
			}
			if (board[loc.first - 1][loc.second - 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(loc.first - 1, loc.second - 1));
			}
		}
	}
	return;
}

class Bishop : public Piece {
public:
	bool captured = false;
	bool givingCheck = false;
	
	Bishop(Color c, char f, int r) : Piece(BISHOP, c, f, r) {}
	Bishop() : Piece(BISHOP, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void Bishop::pieceVision(vector<vector<Piece>> board) {
	pair<int, int> loc = rftcd(location);
	int rotos = 0; // Rotations of the bishop
	while (rotos < 4) {
		switch (rotos) {
		case 0:
			//up-right
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first + i == 8 || loc.second + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first + i][loc.second + i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second + i));
				}
			}
		case 1:
			//down-right
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first - i + 1 == 0 || loc.second + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first - i][loc.second + i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second + i));
				}
			}
		case 2:
			//down-left
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first - i + 1 == 0 || loc.second - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first - i][loc.second - i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second - i));
				}
			}
		case 3:
			//up-left
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first + i == 8 || loc.second - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first + i][loc.second - i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second - i));
				}
			}
			break;
		}
	}
	return;
}

class Knight : public Piece {
public:
	bool captured = false;
	bool givingCheck = false;
	
	Knight(Color c, string loc) : Piece(KNIGHT, c, loc[0], loc[1] - '0') {}
	Knight() : Piece(KNIGHT, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void Knight::pieceVision(vector<vector<Piece>> board) {
	pair<int, int> loc = rftcd(location);
	for (size_t i = 0; i < 8; i++)
	{
		switch (i) {
		case 0:
			//NNE
			if (loc.first <= 5 && loc.second <= 6)
			{
				if (board[loc.first + 2][loc.second + 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 2, loc.second + 1));
				}
			}
			break;
		case 1:
			//NEE
			if (loc.first <= 6 && loc.second <= 5)
			{
				if (board[loc.first + 1][loc.second + 2].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 1, loc.second + 2));
				}
			}
			break;
		case 2:
			//SEE
			if (loc.first >= 1 && loc.second <= 5)
			{
				if (board[loc.first - 1][loc.second + 2].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 1, loc.second + 2));
				}
			}
			break;
		case 3:
			//SSE
			if (loc.first >= 2 && loc.second <= 6)
			{
				if (board[loc.first - 2][loc.second + 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 2, loc.second + 1));
				}
			}
			break;
		case 4:
			//SSW
			if (loc.first >= 2 && loc.second >= 1)
			{
				if (board[loc.first - 2][loc.second - 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 2, loc.second - 1));
				}
			}
			break;
		case 5:
			//SWW
			if (loc.first >= 1 && loc.second >= 2)
			{
				if (board[loc.first - 1][loc.second - 2].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 1, loc.second - 2));
				}	
			}
			break;
		case 6:
			//NWW
			if (loc.first <= 6 && loc.second >= 2)
			{
				if (board[loc.first + 1][loc.second - 2].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 1, loc.second - 2));
				}
			}
			break;
		case 7:
			//NNW
			if (loc.first <= 5 && loc.second >= 1)
			{
				if (board[loc.first + 2][loc.second - 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 2, loc.second - 1));
				}
			}
			break;
		}
	}
	return;
}

class Rook : public Piece {
public:
	bool captured = false;
	bool givingCheck = false;
	
	Rook(Color c, char f, int r) : Piece(ROOK, c, f, r) {}
	Rook() : Piece(ROOK, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void Rook::pieceVision(vector<vector<Piece>>board) {
	pair<int, int> loc = rftcd(location);
	int rotos = 0; // Rotations of the bishop
	while (rotos < 4) {
		switch (rotos) {
		case 0:
			//up
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first + i][loc.second];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first + i, loc.second));
				}
			}
		case 1:
			//right
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.second + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first][loc.second + i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second + i));
				}
			}
		case 2:
			//down
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.first - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first - i][loc.second];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first - i, loc.second));
				}
			}
		case 3:
			//left
			for (size_t i = 1; i < 8; i++)
			{
				if (loc.second - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[loc.first][loc.second - i];
				if (temp.color == color)
				{
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second - i));
				}
			}
			break;
		}
	}
	return;
}

class Queen : public Piece {
public:
	bool captured = false;
	bool givingCheck = false;
	
	Queen(Color c, string loc) : Piece(QUEEN, c, loc[0], loc[1] - '0') {}
	Queen() : Piece(QUEEN, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void Queen::pieceVision(vector<vector<Piece>> board) {
	Bishop b(color, location.first, location.second);
	Rook r(color, location.first, location.second);
	b.pieceVision(board);
	r.pieceVision(board);
	possibleMoves.insert(possibleMoves.end(), b.possibleMoves.begin(), b.possibleMoves.end());
	possibleMoves.insert(possibleMoves.end(), r.possibleMoves.begin(), r.possibleMoves.end());
	return;
}

class King : public Piece {
public:
	bool inCheck = false;
	bool inCheckmate = false;
	
	
	King(Color c, string loc) : Piece(KING, c, loc[0], loc[1] - '0') {}
	King() : Piece(KING, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> board);
};

void King::pieceVision(vector<vector<Piece>> board) {
	pair<int, int> loc = rftcd(location);
	for (size_t i = 0; i < 8; i++)
	{
		switch (i) {
		case 0:
			//N
			if (loc.first <= 6)
			{
				if (board[loc.first + 1][loc.second].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 1, loc.second));
				}
			}
			break;
		case 1:
			//NE
			if (loc.first <= 6 && loc.second <= 6)
			{
				if (board[loc.first + 1][loc.second + 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 1, loc.second + 1));
				}
			}
			break;
		case 2:
			//E
			if (loc.second <= 6)
			{
				if (board[loc.first][loc.second + 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second + 1));
				}
			}
			break;
		case 3:
			//SE
			if (loc.first >= 1 && loc.second <= 6)
			{
				if (board[loc.first - 1][loc.second + 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 1, loc.second + 1));
				}
			}
			break;
		case 4:
			//S
			if (loc.first >= 1)
			{
				if (board[loc.first - 1][loc.second].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 1, loc.second));
				}
			}
			break;
		case 5:
			//SW
			if (loc.first >= 1 && loc.second >= 1)
			{
				if (board[loc.first - 1][loc.second - 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first - 1, loc.second - 1));
				}
			}
			break;
		case 6:
			//W
			if (loc.second >= 1)
			{
				if (board[loc.first][loc.second - 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first, loc.second - 1));
				}
			}
			break;
		case 7:
			//NW
			if (loc.first <= 6 && loc.second >= 1)
			{
				if (board[loc.first + 1][loc.second - 1].color != color)
				{
					possibleMoves.push_back(make_pair(loc.first + 1, loc.second - 1));
				}
			}
			break;
		}
	}
	return;
}



class Player {
public:
	string name;
	Color color;
	bool inCheck = false;
	Player(string n, Color c) : name(n), color(c) {}
	Player() : name(""), color(NULL_COLOR) {}
};

