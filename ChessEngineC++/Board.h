#pragma once

#include "BoardConstants.h"
#include "Moves.h"

class Board {
public:
	static const int BOARD_SIZE = 8;

	Piece pieces[BOARD_SIZE*BOARD_SIZE];
	bool active;
	bool castling[4];
	Position enpassant;
	int halfmove;
	int fullmove;



	Board() { init();  };
	Board(const Board* board) { init(board); };

	void init();
	void init(const Board* board);

	void set(Position pos, Piece piece);
	void set(int x, int y, Piece piece);
	Piece get(Position pos) const;
	Piece get(int x, int y) const;

	void makeMove(Move move);
	void getValidMoves(std::vector<Move>& moves, bool invert = false) const;

	float evaluate() const;
	
	void render();
};