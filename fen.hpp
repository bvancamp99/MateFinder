#pragma once

#include <unordered_set>
#include "color.hpp"

extern const char *START_FEN;
extern const size_t FEN_BUF_SIZE;
extern const std::unordered_set<char> PIECE_LETTERS;

class FenClass {
public:
    char piece_placement[8][8]{};
    char active_color;
    char castling_rights[5]; // 4 possible rights + null terminator
    char en_passant_target[3]; // up to 2 chars for square + null terminator
    long halfmove_clock;
    long fullmove_number;

    FenClass(char *fen_s);
    void to_string(char *dest_buf, size_t buf_size);
};
