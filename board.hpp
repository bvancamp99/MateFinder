#pragma once

#include <cstdint>
#include "basic_buffer.hpp"
#include "color.hpp"
#include "piece.hpp"
#include "square.hpp"

#define MAX_SINGLE_SQUARE_ATTACKS 16 // https://chess.stackexchange.com/questions/44853/whats-the-maximum-number-of-pieces-attacking-a-single-square

class Board {
public:
	BasicBuffer<Piece*, MAX_SINGLE_SQUARE_ATTACKS> attack_table[8][8];
    FenClass *fen_class;

    Board(FenClass *fen_class, Color user_color);
    Piece* get_piece(Square square);
    bool has_piece(Square square);
    bool has_piece(Square square, Color color);

private:
    Color user_color;
    Piece *arr[8][8];
};