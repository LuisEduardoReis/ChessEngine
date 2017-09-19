#include "Board.h"
#include <iostream>

using namespace std;

void Board::init() {
	for (int i = 0; i < Board::BOARD_SIZE*Board::BOARD_SIZE; i++) pieces[i] = E;
	
	set(0, 0, W_R);
	set(1, 0, W_N);
	set(2, 0, W_B);
	set(3, 0, W_Q);
	set(4, 0, W_K);
	set(5, 0, W_B);
	set(6, 0, W_N);
	set(7, 0, W_R);
	for (int i = 0; i < BOARD_SIZE; i++) set(i, 1, W_P);

	set(0, 7, B_R);
	set(1, 7, B_N);
	set(2, 7, B_B);
	set(3, 7, B_Q);
	set(4, 7, B_K);
	set(5, 7, B_B);
	set(6, 7, B_N);
	set(7, 7, B_R);
	for (int i = 0; i < BOARD_SIZE; i++) set(i, 6, B_P);

	active = true;
	for (int i = 0; i < 4; i++) castling[i] = true;

	enpassant.set(-1, -1);
	
	halfmove = 0;
	fullmove = 1;
}

void Board::init(const Board* board) {
	for (int i = 0; i < Board::BOARD_SIZE*Board::BOARD_SIZE; i++) pieces[i] = board->pieces[i];
	active = board->active;
	for (int i = 0; i < 4; i++) castling[i] = board->castling[i];

	enpassant.set(board->enpassant.x, board->enpassant.y);

	halfmove = board->halfmove;
	fullmove = board->fullmove;
};


void Board::set(Position pos, Piece piece) { set(pos.x, pos.y, piece); }
void Board::set(int x, int y, Piece piece) { if (x >= 0 && x < Board::BOARD_SIZE && y >= 0 && y < Board::BOARD_SIZE) pieces[y*BOARD_SIZE + x] = piece; }
Piece Board::get(Position pos) const { return get(pos.x, pos.y); }
Piece Board::get(int x, int y) const { if (x >= 0 && x < Board::BOARD_SIZE && y >= 0 && y < Board::BOARD_SIZE) return pieces[y*BOARD_SIZE + x]; else return N;}


void Board::render() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << PIECES_CHAR.at(get(j,i)) << ' ';
		}
		cout << endl;
	}
};


float Board::evaluate() const {
	int counts[16];
	for (int i = 0; i < 12; i++) { counts[PIECE_TYPES[i]] = 0; }
	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		if (pieces[i] == E) continue;
		counts[pieces[i]] += 1;
	}

	float material =
		200 * (counts[W_K] - counts[B_K]) +
		9 * (counts[W_Q] - counts[B_Q]) +
		5 * (counts[W_R] - counts[B_R]) +
		3 * (counts[W_B] - counts[B_B]) +
		3 * (counts[W_N] - counts[B_N]) +
		1 * (counts[W_P] - counts[B_P]);

	return active ? material : -material;
}