#include "BoardConstants.h"

using namespace std;

bool piece_color(Piece piece) { return (piece & 8) == 0; }
Piece piece_type(Piece piece) { return (Piece)(piece & (~8)); }
Piece set_piece_color(Piece piece, bool color) {
	piece = (Piece)(piece & (~8));
	return (Piece)(color ? piece : (piece & 8));
}

map<int, char> create_map_FILE_INDICES() {
	map<int, char> m;
	m[0] = 'A';
	m[1] = 'B';
	m[2] = 'C';
	m[3] = 'D';
	m[4] = 'E';
	m[5] = 'F';
	m[6] = 'G';
	m[7] = 'H';
	return m;
}
const map<int, char> FILE_INDICES = create_map_FILE_INDICES();

map<Piece, char> create_map_PIECES_CHAR() {
	map<Piece, char> m;
	m[E] = ' ';

	m[W_K] = 'K';
	m[W_Q] = 'Q';
	m[W_R] = 'R';
	m[W_B] = 'B';
	m[W_N] = 'N';
	m[W_P] = 'P';

	m[B_K] = 'k';
	m[B_Q] = 'q';
	m[B_R] = 'r';
	m[B_B] = 'b';
	m[B_N] = 'n';
	m[B_P] = 'p';
	return m;
}
const map<Piece, char> PIECES_CHAR = create_map_PIECES_CHAR();

const int CASTLING_W_K = 0;
const int CASTLING_W_Q = 1;
const int CASTLING_B_K = 2;
const int CASTLING_B_Q = 3;

const Piece PIECE_TYPES[12] = {
	W_P, W_R, W_N, W_B, W_Q, W_K,
	B_P, B_R, B_N, B_B, B_Q, B_K
};