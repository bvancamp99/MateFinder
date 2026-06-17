#pragma once

#include "color.hpp"
#include "piece.hpp"
#include "square.hpp"


class Board {
public:
    FenClass *fen_class;

    Board(FenClass *fen_class, Color user_color);
    Piece* get_piece(Square square);
    bool has_piece(Square square);
    bool has_piece(Square square, Color color);

private:
    Color user_color;
    Piece *arr[8][8];
};