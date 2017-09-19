#include "Search.h"
#include <limits>

using namespace std;

float Search::quiesce(const Board& board, float alpha, float beta) {
	float stand_pat = board.evaluate();
	if (stand_pat >= beta) return beta;
	if (alpha < stand_pat) alpha = stand_pat;

	vector<Move> moves(100);
	board.getValidMoves(moves);
	for (size_t i = 0; i < moves.size(); i++) {
		if (moves[i].isQuiet()) continue;
		search_count++;
		Board next_board(board);
		next_board.makeMove(moves[i]);
		float score = -quiesce(next_board, -beta, -alpha);
		if (score >= beta) return beta;
		if (score > alpha) alpha = score;
	}
	return alpha;
}



float Search::negaMaxIter(const Board& board, int depth, float alpha, float beta) {
	if (depth == 0) return board.evaluate();//quiesce(board, alpha, beta);
	vector<Move> moves(100);
	board.getValidMoves(moves);

	for (size_t i = 0; i < moves.size(); i++) {
		search_count++;
		Board next_board(board);
		next_board.makeMove(moves[i]);
		float score = -negaMaxIter(next_board, depth - 1, -beta, -alpha);
		if (score >= beta) return beta;
		if (score > alpha) alpha = score;
	}

	return alpha;
}

void Search::negaMax(const Board& board, int depth) {
	float alpha = numeric_limits<float>::min();
	float beta = numeric_limits<float>::max();
	search_count = 0;
	
	vector<Move> moves(100);
	board.getValidMoves(moves);
	vector<Move> best;

	for (size_t i = 0; i < moves.size(); i++) {
		Board next_board(board);
		next_board.makeMove(moves[i]);
		float score = -negaMaxIter(next_board, depth - 1, -beta, -alpha);

		if (score > alpha) {
			alpha = score;
			best.clear();
		} else if (score == alpha) 
			best.push_back(moves[i]);
	}
	
	this->max = alpha;
	this->max_moves = best;
}

