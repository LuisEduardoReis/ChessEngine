#include "Moves.h"
#include "Board.h"

using namespace std;

void add_quiet_promotions(vector<Move>& moves, Position from, Position to) {
	moves.push_back(Move(from, to, M_PROMOTE_N));
	moves.push_back(Move(from, to, M_PROMOTE_B));
	moves.push_back(Move(from, to, M_PROMOTE_R));
	moves.push_back(Move(from, to, M_PROMOTE_Q));
}

void add_cap_promotions(vector<Move>& moves, Position from, Position to) {
	moves.push_back(Move(from, to, M_CAP_PROMOTE_N));
	moves.push_back(Move(from, to, M_CAP_PROMOTE_B));
	moves.push_back(Move(from, to, M_CAP_PROMOTE_R));
	moves.push_back(Move(from, to, M_CAP_PROMOTE_Q));
}

bool check_add_move(vector<Move>& moves, bool color, const Board& board, Position from, Position to) {
	Piece tp = board.get(to);
	if (tp == N) return true;
	if (tp == E) {
		moves.push_back(Move(from, to, M_QUIET));
		return false;
	} else {
		if (piece_color(tp) != color) moves.push_back(Move(from, to, M_CAPTURE));
		return true;
	}
}

void add_orthogonal_slide_moves(vector<Move>& moves, bool color, const Board& board, int x, int y) {
	for (int i = y + 1; i < Board::BOARD_SIZE; i++) 	if (check_add_move(moves, color, board, Position(x, y), Position(x, i))) break;
	for (int i = y - 1; i >= 0; i--) 					if (check_add_move(moves, color, board, Position(x, y), Position(x, i))) break;
	for (int i = x + 1; i < Board::BOARD_SIZE; i++) 	if (check_add_move(moves, color, board, Position(x, y), Position(i, y))) break;
	for (int i = x - 1; i >= 0; i--) 					if (check_add_move(moves, color, board, Position(x, y), Position(i, y))) break;
}

void add_diagonal_slide_moves(vector<Move>& moves, bool color, const Board& board, int x, int y) {
	for (int i = 1; x + i < Board::BOARD_SIZE && y + i < Board::BOARD_SIZE; i++) 	if (check_add_move(moves, color, board, Position(x, y), Position(x + i, y + i))) break;
	for (int i = 1; x - i >= 0 && y + i < Board::BOARD_SIZE; i++) 					if (check_add_move(moves, color, board, Position(x, y), Position(x - i, y + i))) break;
	for (int i = 1; x + i < Board::BOARD_SIZE && y + i >= 0; i++) 					if (check_add_move(moves, color, board, Position(x, y), Position(x + i, y - i))) break;
	for (int i = 1; x - i >= 0 && y - i >= 0; i++) 									if (check_add_move(moves, color, board, Position(x, y), Position(x - i, y - i))) break;
}

vector<Position> create_knight_moves() {
	vector<Position> v;
	v.push_back(Position(-2, -1));
	v.push_back(Position(-2,  1));
	v.push_back(Position(-1, -2));
	v.push_back(Position(-1,  2));
	v.push_back(Position( 1, -2));
	v.push_back(Position( 1,  2));
	v.push_back(Position( 2, -1));
	v.push_back(Position( 2,  1));
	return v;
}
vector<Position> KNIGHT_MOVES = create_knight_moves();

vector<Position> create_king_moves() {
	vector<Position> v;
	v.push_back(Position( 0, -1));
	v.push_back(Position( 0,  1));
	v.push_back(Position( 1,  0));
	v.push_back(Position(-1,  0));
	v.push_back(Position( 1,  1));
	v.push_back(Position(-1,  1));
	v.push_back(Position( 1, -1));
	v.push_back(Position(-1, -1));
	return v;
}
vector<Position> KING_MOVES = create_king_moves();

map<MoveType, Piece> create_promotion_pieces() {
	map<MoveType, Piece> m;
	m[M_PROMOTE_R] = W_R;
	m[M_PROMOTE_N] = W_N;
	m[M_PROMOTE_B] = W_B;
	m[M_PROMOTE_Q] = W_Q;

	m[M_CAP_PROMOTE_R] = W_R;
	m[M_CAP_PROMOTE_N] = W_N;
	m[M_CAP_PROMOTE_B] = W_B;
	m[M_CAP_PROMOTE_Q] = W_Q;
	return m;
}
const map<MoveType, Piece> PROMOTION_PIECES = create_promotion_pieces();


void Board::getValidMoves(vector<Move>& moves, bool invert) const {

	moves.clear();

	bool c = invert ? !active : active;
	int forward = c ? 1 : -1,
		first_rank = c ? 1 : Board::BOARD_SIZE-2,
		last_rank_before_promote = c ? Board::BOARD_SIZE - 2 : 1,
		king_rank = c ? 0 : Board::BOARD_SIZE - 1;
	bool has_king = false;

	// Regular moves
	for (int y = 0; y < Board::BOARD_SIZE; y++) {
		for (int x = 0; x < Board::BOARD_SIZE; x++) {
			Piece p = get(x, y);
			if (p == E) continue;
			bool pc = piece_color(p);
			if (pc != c) continue;
			Piece pt = piece_type(p);

			Piece fp, f2p, lp, rp;

			// Regular moves
			switch (pt) {
			case W_P: // Pawn
				fp = get(x, y + forward);
				f2p = get(x, y + 2 * forward);
				lp = get(x - 1, y + forward);
				rp = get(x + 1, y + forward);
				if (y != last_rank_before_promote) {
					//Push forward
					if (fp == E) 
						moves.push_back(Move(Position(x, y), Position(x, y + forward), M_QUIET));
					//Double push
					if (y == first_rank && fp == E && f2p == E) 
						moves.push_back(Move(Position(x, y), Position(x, y + 2 * forward), M_DOUBLE_PAWN_PUSH));
					//Left Capture
					if (lp != N && lp != E && piece_color(lp) != c)
						moves.push_back(Move(Position(x, y), Position(x - 1, y + forward), M_CAPTURE));
					//Right Capture
					if (rp != N && rp != E && piece_color(rp) != c) 
						moves.push_back(Move(Position(x, y), Position(x + 1, y + forward), M_CAPTURE));
					// En Passant Left
					if (!enpassant.isNull() && lp == E && enpassant.x == x - forward && enpassant.y == y + forward)
						moves.push_back(Move(Position(x, y), Position(x - forward, y + forward), M_ENCAPTURE));
					// En Passant Right
					if (!enpassant.isNull() && rp == E && enpassant.x == x + forward && enpassant.y == y + forward)
						moves.push_back(Move(Position(x, y), Position(x + forward, y + forward), M_ENCAPTURE));
				} else {
					// Push Forward
					if (fp == E) add_quiet_promotions(moves, Position(x, y), Position(x, y + forward));
					// Left Capture
					if (lp != N && lp != E && piece_color(lp) != c) 
						add_cap_promotions(moves, Position(x, y), Position(x - forward, y + forward));
					// Right Capture
					if (rp != N && rp != E && piece_color(rp) != c) 
						add_cap_promotions(moves, Position(x, y), Position(x + forward, y + forward));
				}
				break;
			case W_R: // Rook
				add_orthogonal_slide_moves(moves, c, this, x, y);
				break;
			case W_N: // Knight
				for(size_t i = 0; i < KNIGHT_MOVES.size(); i++) {
					Position m = KNIGHT_MOVES[i];
					check_add_move(moves, c, this, Position(x,y), Position(x + m.x, y + m.y));
				}
				break;
			case W_B: // Bishop
				add_diagonal_slide_moves(moves, c, this, x, y);
				break;
			case W_Q: //Queen
				add_orthogonal_slide_moves(moves, c, this, x, y);
				add_diagonal_slide_moves(moves, c, this, x, y);
				break;
			case W_K: //King
				has_king = true;
				for (size_t i = 0; i < KING_MOVES.size(); i++) {
					Position m = KING_MOVES[i];
					check_add_move(moves, c, this, Position(x, y), Position(x + m.x, y + m.y));
				}
				break;
			}
		}
	}
	if (!has_king) {
		moves.clear();
		return;
	}

	// Castling
	Piece king = c ? W_K : B_K,
		rook = c ? W_R : B_R;
	// King castling			
	if (castling[c ? CASTLING_W_K : CASTLING_B_K] &&
		get(4, king_rank) == king &&
		get(5, king_rank) == E &&
		get(6, king_rank) == E &&
		get(7, king_rank) == rook) {
		moves.push_back(Move(Position(), Position(), M_KCASTLE));
	}
	// King castling			
	if (castling[c ? CASTLING_W_Q : CASTLING_B_Q] &&
		get(0, king_rank) == rook &&
		get(1, king_rank) == E &&
		get(2, king_rank) == E &&
		get(3, king_rank) == E &&
		get(4, king_rank) == king) {
		moves.push_back(Move(Position(), Position(), M_QCASTLE));
	}

}


void Board::makeMove(Move move) {
	MoveType t = move.type;
	bool c = active;
	Piece mp = get(move.from);
	enpassant = Position();

	int rank;
	Piece pp;

	switch (t) {
	case M_QUIET:
	case M_CAPTURE:
		//Revoke Castling Rights
		if (piece_type(mp) == W_K) {
			castling[c ? CASTLING_W_K : CASTLING_B_K] = false;
			castling[c ? CASTLING_W_Q : CASTLING_B_Q] = false;
		}
		if (piece_type(mp) == W_R && move.from.y == (c ? 0 : 7)) {
			if (move.from.x == 0) castling[c ? CASTLING_W_Q : CASTLING_B_Q] = false;
			if (move.from.x == 7) castling[c ? CASTLING_W_K : CASTLING_B_K] = false;
		}
		set(move.from, E);
		set(move.to, mp);
		break;
	case M_KCASTLE:
		rank = c ? 0 : 7;
		castling[c ? CASTLING_W_K : CASTLING_B_K] = false;
		set(4, rank, E);
		set(5, rank, c ? W_R : B_R);
		set(6, rank, c ? W_K : B_K);
		set(7, rank, E);
		break;
	case M_QCASTLE:
		rank = c ? 0 : 7;
		castling[c ? CASTLING_W_Q : CASTLING_B_Q] = false;
		set(0, rank, E);
		set(2, rank, c ? W_K : B_K);
		set(3, rank, c ? W_R : B_R);
		set(4, rank, E);
		break;
	case M_DOUBLE_PAWN_PUSH:
		set(move.from, E);
		set(move.to, mp);
		enpassant = Position(move.from.x, move.from.y + (c ? 1 : -1));
		break;
	case M_ENCAPTURE: // Todo test
		set(move.from, E);
		set(move.to, mp);
		set(move.to.x, move.to.y + (c ? -1 : 1), E);
		break;
	case M_PROMOTE_N:
	case M_PROMOTE_B:
	case M_PROMOTE_R:
	case M_PROMOTE_Q:
	case M_CAP_PROMOTE_N:
	case M_CAP_PROMOTE_B:
	case M_CAP_PROMOTE_R:
	case M_CAP_PROMOTE_Q:
		pp = set_piece_color(PROMOTION_PIECES.at(t), c);
		set(move.from, E);
		set(move.to, pp);
		break;
	}

	if (!active) fullmove++;
	halfmove++;
	if (t == M_CAPTURE || piece_type(mp) == W_P) halfmove = 0;

	active = !active;
}