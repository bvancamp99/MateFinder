#pragma once

#include <cstdint>
#include "color.hpp"
#include "square.hpp"

#define OPP_COLOR(color) { color ^ 1 }

typedef uint64_t bitboard_t;

enum class PieceType : uint8_t {
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
	static const bitboard_t lookup_table[NUM_SQUARES];

	King(Color color, Square square);
};

class Knight : Piece {
public:
	static const bitboard_t lookup_table[NUM_SQUARES];

	Knight(Color color, Square square);
};

class WhitePawn : Piece {
public:
	static const bitboard_t push_lookup_table[NUM_SQUARES];
	static const bitboard_t capture_lookup_table[NUM_SQUARES];

	WhitePawn(Square square);
};

class BlackPawn : Piece {
public:
	static const bitboard_t push_lookup_table[NUM_SQUARES];
	static const bitboard_t capture_lookup_table[NUM_SQUARES];

	BlackPawn(Square square);
};
