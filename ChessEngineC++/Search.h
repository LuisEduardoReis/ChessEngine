#pragma once

#include "Moves.h"
#include "Board.h"

#include <iostream>

class Search {
public:
	float max;
	std::vector<Move> max_moves;
	int search_count;

	void negaMax(const Board& board, int depth);
	
private:
	float negaMaxIter(const Board& board, int depth, float alpha, float beta);
	float quiesce(const Board& board, float alpha, float beta);
};

