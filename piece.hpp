#pragma once

#include <vector>
#include "basic_buffer.hpp"
#include "board.hpp"
#include "color.hpp"
#include "square.hpp"

#define OPP_COLOR(color) { color ^ 1 }

enum class PieceType : unsigned char {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

class Piece {
public:
	PieceType type;
	Color color;
    Square square;

	Piece(PieceType type, Color color, Square square);
};

class King : Piece {
public:
	static bitboard_t lookup_table[NUM_RANKS][NUM_FILES];

	King(Color color, Square square);
};

class Knight : Piece {
public:
	static bitboard_t lookup_table[NUM_RANKS][NUM_FILES];

	Knight(Color color, Square square);
};
