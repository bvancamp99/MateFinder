#pragma once

#include "color.hpp"

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

// TODO: letter to color function

class Piece {
public:
    char letter;
    Square square;
    Color color;
    Color opp_color;
    PieceType type;

    Piece(char letter, Square square);
};
