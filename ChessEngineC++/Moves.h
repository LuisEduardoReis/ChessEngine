#pragma once

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "BoardConstants.h"


enum MoveType {
	M_QUIET = 0,
	M_DOUBLE_PAWN_PUSH = 1,
	M_KCASTLE = 2,
	M_QCASTLE = 3,
	M_CAPTURE = 4,
	M_ENCAPTURE = 5,
	M_PROMOTE_N = 8,
	M_PROMOTE_B = 9,
	M_PROMOTE_R = 10,
	M_PROMOTE_Q = 11,
	M_CAP_PROMOTE_N = 12,
	M_CAP_PROMOTE_B = 13,
	M_CAP_PROMOTE_R = 14,
	M_CAP_PROMOTE_Q = 15
};

extern const std::map<MoveType, Piece> PROMOTION_PIECES;

class Move {
public:
	Position from, to;
	MoveType type;
	Move() {};
	Move(Position from, Position to, MoveType type) {
		this->from = from;
		this->to = to;
		this->type = type;
	}
	
	bool isQuiet() { return (type & 4) == 0; }

	std::string toString() {
		if (type == M_KCASTLE) return "0-0";
		if (type == M_QCASTLE) return "0-0-0";
		std::stringstream ss;
		ss << (char)(65 + from.x) << (from.y + 1);
		ss << (isQuiet() ? ' ' : 'x');
		ss << (char)(65 + to.x) << (to.y + 1);		
		return ss.str();
	}
};
