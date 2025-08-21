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
	char c1 = 'a' + (loc.second); // Convert column index to file
	char c2 = (loc.first + 1) + '0';
	string s;
	s.push_back(c1);
	s.push_back(c2);
	return s;
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
	bool hasMoved = false; // pawns and castling rights
	bool givingCheck = false; // whether the piece is giving check
	bool isProtected = false; // whether the piece is protected by another piece
	vector<pair<int, int>> pathForCheck;
	Piece(PieceType t, Color c, char f, int r) {
		type = t;
		color = c;
		location = make_pair(f, r);
	}
	Piece() : type(EMPTY), color(NULL_COLOR), location(make_pair('a', 1)) {}
	string toString();
	string locString() { return string(1, location.first) + to_string(location.second); }
	string toStringBoard();
	virtual void pieceVision(vector<vector<Piece>> &board) {}
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
	
	Pawn(Color c, char f, int r) : Piece(PAWN, c, f, r) {}
	Pawn() : Piece(PAWN, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> &board, bool morv);
};

void Pawn::pieceVision(vector<vector<Piece>> &board, bool morv) {
	pair<int, int> loc = rftcd(location);
	int l1 = loc.first;
	int l2 = loc.second;
	if (color == WHITE)
	{
		if (morv == false)
		{
			if (l2 == 0)
			{
				if (board[l1 + 1][l2 + 1].color == WHITE)
				{
					board[l1 + 1][l2 + 1].isProtected = true;
				}
				if (board[l1 + 1][l2 + 1].color == BLACK && board[l1 + 1][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
			}
			if (l2 == 7)
			{
				if (board[l1 + 1][l2 - 1].color == WHITE)
				{
					board[l1 + 1][l2 - 1].isProtected = true;
				}
				if (board[l1 + 1][l2 - 1].color == BLACK && board[l1 + 1][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
			}
			if (l2 > 0 && l2 < 7)
			{
				if (board[l1 + 1][l2 + 1].color == WHITE)
				{
					board[l1 + 1][l2 + 1].isProtected = true;
				}
				if (board[l1 + 1][l2 + 1].color == BLACK && board[l1 + 1][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				if (board[l1 + 1][l2 - 1].color == WHITE)
				{
					board[l1 + 1][l2 - 1].isProtected = true;
				}
				if (board[l1 + 1][l2 - 1].color == BLACK && board[l1 + 1][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
				possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
			}
			return;
		}
		if (board[l1 + 1][l2].type == EMPTY)
		{
			possibleMoves.push_back(make_pair(l1 + 1, l2));
			if (hasMoved == false)
			{
				if (board[l1 + 2][l2].type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 + 2, l2));
				}
			}
		}
		if (l2 == 0)
		{
			if (board[l1 + 1][l2 + 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
			}
		}
		if (l2 == 7)
		{
			if (board[l1 + 1][l2 - 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
			}
		}
		if (l2 > 0 && l2 < 7)
		{
			if (board[l1 + 1][l2 + 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
			}
			if (board[l1 + 1][l2 - 1].color == BLACK)
			{
				possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
			}
		}
	}
	if (color == BLACK)
	{
		if (morv == false)
		{
			if (l2 == 0)
			{
				if (board[l1 - 1][l2 + 1].color == BLACK)
				{
					board[l1 - 1][l2 + 1].isProtected = true;
				}
				if (board[l1 - 1][l2 + 1].color == WHITE && board[l1 - 1][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
			}
			if (l2 == 7)
			{
				if (board[l1 - 1][l2 - 1].color == BLACK)
				{
					board[l1 - 1][l2 - 1].isProtected = true;
				}
				if (board[l1 - 1][l2 - 1].color == WHITE && board[l1 - 1][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
			}
			if (l2 > 0 && l2 < 7)
			{
				if (board[l1 - 1][l2 + 1].color == BLACK)
				{
					board[l1 - 1][l2 + 1].isProtected = true;
				}
				if (board[l1 - 1][l2 + 1].color == WHITE && board[l1 - 1][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				if (board[l1 - 1][l2 - 1].color == BLACK)
				{
					board[l1 - 1][l2 - 1].isProtected = true;
				}
				if (board[l1 - 1][l2 - 1].color == WHITE && board[l1 - 1][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
				possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
				possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
			}
			return;
		}
		if (board[l1 - 1][l2].type == EMPTY)
		{
			possibleMoves.push_back(make_pair(l1 - 1, l2));
			if (hasMoved == false)
			{
				if (board[l1 - 2][l2].type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 - 2, l2));
				}
			}
		}
		if (l2 == 0)
		{
			if (board[l1 - 1][l2 + 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
			}
		}
		if (l2 == 7)
		{
			if (board[l1 - 1][l2 - 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
			}
		}
		if (l2 > 0 && l2 < 7)
		{
			if (board[l1 - 1][l2 + 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
			}
			if (board[l1 - 1][l2 - 1].color == WHITE)
			{
				possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
			}
		}
	}
	return;
}

class Bishop : public Piece {
public:
	bool captured = false;
	
	
	Bishop(Color c, char f, int r) : Piece(BISHOP, c, f, r) {}
	Bishop() : Piece(BISHOP, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> &board, bool morv);
};

void Bishop::pieceVision(vector<vector<Piece>> &board, bool morv) {
	pair<int, int> loc = rftcd(location);
	int l1 = loc.first;
	int l2 = loc.second;
	int rotos = 0; // Rotations of the bishop
	while (rotos < 4) {
		switch (rotos) {
		case 0:
			//up-right
			for (int i = 1; i < 8; i++)
			{
				if (l1 + i == 8 || l2 + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 + i][l2 + i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 + i][l2 + i].isProtected = true;
						possibleMoves.push_back(make_pair(l1 + i, l2 + i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 + j][l2 + j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 + j, l2 + j));
						}
					}
					possibleMoves.push_back(make_pair(l1 + i, l2 + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 + i, l2 + i));
				}
			}
		case 1:
			//down-right
			for (int i = 1; i < 8; i++)
			{
				if (l1 - i + 1 == 0 || l2 + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 - i][l2 + i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 - i][l2 + i].isProtected = true;
						possibleMoves.push_back(make_pair(l1 - i, l2 + i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 - j][l2 + j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 - j, l2 + j));
						}
					}
					possibleMoves.push_back(make_pair(l1 - i, l2 + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 - i, l2 + i));
				}
			}
		case 2:
			//down-left
			for (int i = 1; i < 8; i++)
			{
				if (l1 - i + 1 == 0 || l2 - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 - i][l2 - i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 - i][l2 - i].isProtected = true;
						possibleMoves.push_back(make_pair(l1 - i, l2 - i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 - j][l2 - j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 - j, l2 - j));
						}
					}
					possibleMoves.push_back(make_pair(l1 - i, l2 - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 - i, l2 - i));
				}
			}
		case 3:
			//up-left
			for (int i = 1; i < 8; i++)
			{
				if (l1 + i == 8 || l2 - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 + i][l2 - i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 + i][l2 - i].isProtected = true;
						possibleMoves.push_back(make_pair(l1 + i, l2 - i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 + j][l2 - j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 + j, l2 - j));
						}
					}
					possibleMoves.push_back(make_pair(l1 + i, l2 - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 + i, l2 - i));
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
	
	
	Knight(Color c, string loc) : Piece(KNIGHT, c, loc[0], loc[1] - '0') {}
	Knight() : Piece(KNIGHT, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> &board, bool morv);
};

void Knight::pieceVision(vector<vector<Piece>> &board, bool morv) {
	pair<int, int> loc = rftcd(location);
	int l1 = loc.first;
	int l2 = loc.second;
	for (int i = 0; i < 8; i++)
	{
		switch (i) {
		case 0:
			//NNE
			if (l1 <= 5 && l2 <= 6)
			{
				if (board[l1 + 2][l2 + 1].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 2, l2 + 1));
				}
				if (board[l1 + 2][l2 + 1].color == color)
				{
					board[l1 + 2][l2 + 1].isProtected = true;
					break;
				}
				if (board[l1 + 2][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 1:
			//NEE
			if (l1 <= 6 && l2 <= 5)
			{
				if (board[l1 + 1][l2 + 2].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 + 2));
				}
				if (board[l1 + 1][l2 + 2].color == color)
				{
					board[l1 + 1][l2 + 2].isProtected = true;
					break;
				}
				if (board[l1 + 1][l2 + 2].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 2:
			//SEE
			if (l1 >= 1 && l2 <= 5)
			{
				if (board[l1 - 1][l2 + 2].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 + 2));
				}
				if (board[l1 - 1][l2 + 2].color == color)
				{
					board[l1 - 1][l2 + 2].isProtected = true;
					break;
				}
				if (board[l1 - 1][l2 + 2].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 3:
			//SSE
			if (l1 >= 2 && l2 <= 6)
			{
				if (board[l1 - 2][l2 + 1].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 2, l2 + 1));
				}
				if (board[l1 - 2][l2 + 1].color == color)
				{
					board[l1 - 2][l2 + 1].isProtected = true;
					break;
				}
				if (board[l1 - 2][l2 + 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 4:
			//SSW
			if (l1 >= 2 && l2 >= 1)
			{
				if (board[l1 - 2][l2 - 1].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 2, l2 - 1));
				}
				if (board[l1 - 2][l2 - 1].color == color)
				{
					board[l1 - 2][l2 - 1].isProtected = true;
					break;
				}
				if (board[l1 - 2][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 5:
			//SWW
			if (l1 >= 1 && l2 >= 2)
			{
				if (board[l1 - 1][l2 - 2].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 - 2));
				}	
				if (board[l1 - 1][l2 - 2].color == color)
				{
					board[l1 - 1][l2 - 2].isProtected = true;
					break;
				}
				if (board[l1 - 1][l2 - 2].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 6:
			//NWW
			if (l1 <= 6 && l2 >= 2)
			{
				if (board[l1 + 1][l2 - 2].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 - 2));
				}
				if (board[l1 + 1][l2 - 2].color == color)
				{
					board[l1 + 1][l2 - 2].isProtected = true;
					break;
				}
				if (board[l1 + 1][l2 - 2].type == KING)
				{
					board[l1][l2].givingCheck = true;
				}
			}
			break;
		case 7:
			//NNW
			if (l1 <= 5 && l2 >= 1)
			{
				if (board[l1 + 2][l2 - 1].color != color || morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 2, l2 - 1));
				}
				if (board[l1 + 2][l2 - 1].color == color)
				{
					board[l1 + 2][l2 - 1].isProtected = true;
					break;
				}
				if (board[l1 + 2][l2 - 1].type == KING)
				{
					board[l1][l2].givingCheck = true;
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
	
	
	Rook(Color c, char f, int r) : Piece(ROOK, c, f, r) {}
	Rook() : Piece(ROOK, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> &board, bool morv);
};

void Rook::pieceVision(vector<vector<Piece>> &board, bool morv) {
	pair<int, int> loc = rftcd(location);
	int l1 = loc.first;
	int l2 = loc.second;
	int rotos = 0; // Rotations of the bishop
	while (rotos < 4) {
		switch (rotos) {
		case 0:
			//up
			for (int i = 1; i < 8; i++)
			{
				if (l1 + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 + i][l2];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 + i][l2].isProtected = true;
						possibleMoves.push_back(make_pair(l1 + i, l2));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 + j][l2].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 + j, l2));
						}
					}
					possibleMoves.push_back(make_pair(l1 + i, l2));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 + i, l2));
				}
			}
		case 1:
			//right
			for (int i = 1; i < 8; i++)
			{
				if (l2 + i == 8)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1][l2 + i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1][l2 + i].isProtected = true;
						possibleMoves.push_back(make_pair(l1, l2 + i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1][l2 + j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1, l2 + j));
						}
					}
					possibleMoves.push_back(make_pair(l1, l2 + i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1, l2 + i));
				}
			}
		case 2:
			//down
			for (int i = 1; i < 8; i++)
			{
				if (l1 - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1 - i][l2];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1 - i][l2].isProtected = true;
						possibleMoves.push_back(make_pair(l1 - i, l2));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1 - j][l2].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1 - j, l2));
						}
					}
					possibleMoves.push_back(make_pair(l1 - i, l2));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1 - i, l2));
				}
			}
		case 3:
			//left
			for (int i = 1; i < 8; i++)
			{
				if (l2 - i + 1 == 0)
				{
					++rotos;
					break;
				}
				Piece temp = board[l1][l2 - i];
				if (temp.color == color)
				{
					if (morv == false)
					{
						board[l1][l2 - i].isProtected = true;
						possibleMoves.push_back(make_pair(l1, l2 - i));
					}
					++rotos;
					break;
				}
				if (temp.type != EMPTY)
				{
					if (temp.type == KING)
					{
						board[l1][l2].givingCheck = true;
						for (int j = 1; j < 8; j++)
						{
							if (board[l1][l2 - j].type == KING)
							{
								break;
							}
							board[l1][l2].pathForCheck.push_back(make_pair(l1, l2 - j));
						}
					}
					possibleMoves.push_back(make_pair(l1, l2 - i));
					++rotos;
					break;
				}
				if (temp.type == EMPTY)
				{
					possibleMoves.push_back(make_pair(l1, l2 - i));
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
	
	
	Queen(Color c, string loc) : Piece(QUEEN, c, loc[0], loc[1] - '0') {}
	Queen() : Piece(QUEEN, NULL_COLOR, 'a', 1) {}
	void pieceVision(vector<vector<Piece>> &board, bool morv);
};

void Queen::pieceVision(vector<vector<Piece>> &board, bool morv) {
	Bishop b(color, location.first, location.second);
	Rook r(color, location.first, location.second);
	b.pieceVision(board, morv);
	r.pieceVision(board, morv);
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
	void pieceVision(vector<vector<Piece>> &board, bool morv, vector<pair<int, int>> oppoVision);
};

void King::pieceVision(vector<vector<Piece>> &board, bool morv, vector<pair<int, int>> oppoVision) {
	pair<int, int> loc = rftcd(location);
	int l1 = loc.first;
	int l2 = loc.second;
	for (int i = 0; i < 8; i++)
	{
		switch (i) {
		case 0:
			//N
			if (l1 <= 6)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2));
					if (board[l1 + 1][l2].color == color)
					{
						board[l1 + 1][l2].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 + 1, l2)) == oppoVision.end() && board[l1 + 1][l2].color != color)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2));
				}
			}
			break;
		case 1:
			//NE
			if (l1 <= 6 && l2 <= 6)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
					if (board[l1 + 1][l2 + 1].color == color)
					{
						board[l1 + 1][l2 + 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 + 1, l2 + 1)) == oppoVision.end() && board[l1 + 1][l2 + 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 + 1));
				}
			}
			break;
		case 2:
			//E
			if (l2 <= 6)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1, l2 + 1));
					if (board[l1][l2 + 1].color == color)
					{
						board[l1][l2 + 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1, l2 + 1)) == oppoVision.end() && board[l1][l2 + 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1, l2 + 1));
				}
			}
			break;
		case 3:
			//SE
			if (l1 >= 1 && l2 <= 6)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
					if (board[l1 - 1][l2 + 1].color == color)
					{
						board[l1 - 1][l2 + 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 - 1, l2 + 1)) == oppoVision.end() && board[l1 - 1][l2 + 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 + 1));
				}
			}
			break;
		case 4:
			//S
			if (l1 >= 1)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2));
					if (board[l1 - 1][l2].color == color)
					{
						board[l1 - 1][l2].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 - 1, l2)) == oppoVision.end() && board[l1 - 1][l2].color != color)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2));
				}
			}
			break;
		case 5:
			//SW
			if (l1 >= 1 && l2 >= 1)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
					if (board[l1 - 1][l2 - 1].color == color)
					{
						board[l1 - 1][l2 - 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 - 1, l2 - 1)) == oppoVision.end() && board[l1 - 1][l2 - 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1 - 1, l2 - 1));
				}
			}
			break;
		case 6:
			//W
			if (l2 >= 1)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1, l2 - 1));
					if (board[l1][l2 - 1].color == color)
					{
						board[l1][l2 - 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1, l2 - 1)) == oppoVision.end() && board[l1][l2 - 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1, l2 - 1));
				}
			}
			break;
		case 7:
			//NW
			if (l1 <= 6 && l2 >= 1)
			{
				if (morv == false)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
					if (board[l1 + 1][l2 - 1].color == color)
					{
						board[l1 + 1][l2 - 1].isProtected = true;
					}
					break;
				}
				if (find(oppoVision.begin(), oppoVision.end(), make_pair(l1 + 1, l2 - 1)) == oppoVision.end() && board[l1 + 1][l2 - 1].color != color)
				{
					possibleMoves.push_back(make_pair(l1 + 1, l2 - 1));
				}
			}
			break;
		}
	}
	return;
}



