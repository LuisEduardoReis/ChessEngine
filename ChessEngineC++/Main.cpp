#include <iostream>
#include <ctime>

#include "Board.h"
#include "Moves.h"
#include "Search.h"

using namespace std;

unsigned long long perft(const Board& board, int depth) {
	if (depth <= 0) return 1;
	unsigned long long c = 0;
	vector<Move> moves;
	board.getValidMoves(moves);

	for (size_t i = 0; i < moves.size(); i++) {
		Board next_board(board);
		next_board.makeMove(moves[i]);
		c+=perft(next_board, depth - 1);
	}

	return c;
}

int main(int argc, char* argv[]) {
	Board board;
	
	/*
	vector<Move> moves;
	for (int i = 0; i < 100000; i++) {
		board.init();
		moves.clear();
		board.getValidMoves(moves);
		board.makeMove(moves[0]);
	}*/


	for (int i = 0; i < 10; i++) {
		Search search;
		clock_t start;
		double duration;
		int states_per_second;

		start = clock();
		search.negaMax(board, 8);
		duration = (clock() - start) / (double)CLOCKS_PER_SEC;
		states_per_second = (int) round(search.search_count / duration);

		cout << "\"" << search.max_moves[0].toString() << "\" " 
			<< search.search_count << " " 
			<< duration << " " 
			<< states_per_second 
			<< endl;

		board.makeMove(search.max_moves[0]);
	}
	cout << endl;
	board.render();
	cin.get();

	return 0;
}
