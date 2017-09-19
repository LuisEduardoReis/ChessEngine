#pragma once
#include <map>
#include <list>

enum Piece {
	N = -1,
	E = 0,

	W_P = 1, 
	W_R = 2, 
	W_N = 3, 
	W_B = 4, 
	W_Q = 5, 
	W_K = 6,

	B_P = 9,
	B_R = 10,
	B_N = 11,
	B_B = 12,
	B_Q = 13,
	B_K = 14	
};

bool piece_color(Piece piece);
Piece piece_type(Piece piece);
Piece set_piece_color(Piece piece, bool color);

extern const std::map<int, char> FILE_INDICES;
extern const std::map<Piece, char> PIECES_CHAR;

extern const Piece PIECE_TYPES[12];

extern const int CASTLING_W_K;
extern const int CASTLING_W_Q;
extern const int CASTLING_B_K;
extern const int CASTLING_B_Q;
extern const int* CASTLING_TYPES;

class Position {
public:
	int x, y;
	bool isNull() const { return x < 0; }
	void set(int x, int y) { this->x = x; this->y = y; }
	void set(const Position& p) { this->x = p.x; this->y = p.y; }
	Position() { set(-1, -1); }
	Position(int x, int y) { set(x, y);  }
};


