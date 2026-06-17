#include "board.hpp"

Board::Board(FenClass *fen_class, Color user_color) {
    this->fen_class = fen_class;
    this->user_color = user_color;
}

Piece* Board::get_piece(Square square) {
    return this->arr[static_cast<int>(square.rank)][static_cast<int>(square.file)];
}

bool Board::has_piece(Square square) {
    return this->get_piece(square) != nullptr;
}

bool Board::has_piece(Square square, Color color) {
    Piece *piece = this->get_piece(square);
    return piece != nullptr && piece->color == color;
}
