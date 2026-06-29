#pragma once

#include <string>
#include <unordered_set>
#include "color.hpp"

#define NUM_FILES 8
#define NUM_RANKS 8
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN_BUF_SIZE 91 // https://chess.stackexchange.com/questions/30004/longest-possible-fen

extern const std::unordered_set<char> PIECE_LETTERS;
extern const std::unordered_set<char> CR_LETTERS;

class FenClass {
public:
    char piece_placement[8][8];
    char active_color;
    char castling_rights[5]; // 4 possible rights + null terminator
    char en_passant_target[3]; // up to 2 chars for square + null terminator
    int halfmove_clock;
    int fullmove_number;

	FenClass(std::string const& fen);
    std::string to_string();
    void print_piece_placement();

private:
	void set_piece_placement(std::string const& fen, size_t delim_pos);
	void set_active_color(std::string const& fen, size_t begin_pos, size_t delim_pos);
	void set_castling_rights(std::string const& fen, size_t begin_pos, size_t delim_pos);
	void set_en_passant_target(std::string const& fen, size_t begin_pos, size_t delim_pos);
	void set_halfmove_clock(std::string const& fen, size_t begin_pos, size_t delim_pos);
	void set_fullmove_number(std::string const& fen, size_t begin_pos);
};
