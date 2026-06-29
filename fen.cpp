#include <cctype>
#include <cstdlib>
#include <cstring>
#include <format>
#include <print>
#include <string>
#include "error.hpp"
#include "fen.hpp"

const std::unordered_set<char> PIECE_LETTERS{'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p'};
const std::unordered_set<char> CR_LETTERS{'K', 'Q', 'k', 'q'};

void FenClass::set_piece_placement(std::string const& fen, size_t delim_pos) {
	// Is there a token?
	if (delim_pos == std::string::npos) { ERROR("space delimiter not found"); }

	// Correct size?
	size_t min_pp = 17; // e.g. 8/8/8/8/8/8/k7/K7
	size_t max_pp = 71; // 64 piece letters + 7 '/' characters
	if (delim_pos < min_pp) { ERROR("too short"); }
	else if (delim_pos > max_pp) { ERROR("too long"); }

	// Set piece_placement while validating substring contents

	int i = 0; // rank index
	int j = 0; // file index
	for (size_t pp_i = 0; pp_i < delim_pos; pp_i++) {
		char pp_c = fen[pp_i];

		// Piece letter
		if (PIECE_LETTERS.contains(pp_c)) {
			// Must not be beyond end of current rank
			if (j >= NUM_FILES) { ERROR(std::format("too many files on rank {}", 8 - i)); }

			// Put letter in piece_placements
			this->piece_placement[i][j++] = pp_c;

		// Number
		} else if (std::isdigit(static_cast<int>(pp_c))) {
			int j_after = j + (pp_c - '0');

			// Must not go more than one space past the end of current rank
			if (j_after > NUM_FILES) { ERROR(std::format("too many files on rank {}", 8 - i)); }

			// Put null char for every empty space
			while (j < j_after) { this->piece_placement[i][j++] = '\0'; }

		// '/' end of rank
		} else if (pp_c == '/') {
			// Must be exactly one space past the end of current rank
			if (j != NUM_FILES) { ERROR(std::format("incorrect number of files on rank {}", 8 - i)); }

			// Can't be at the final rank
			if (++i >= NUM_RANKS) { ERROR("too many ranks, i.e. '/' characters"); }

			// Reset j to begin next rank
			j = 0;

		// Any other character is invalid
		} else { ERROR(std::format("invalid character '{}'", pp_c)); }
	}
	// Check that i and j made it to the expected end
	if (i < NUM_RANKS - 1) { ERROR("too few ranks"); }
	else if (j < NUM_FILES) { ERROR("too few files on rank 1"); }
}

void FenClass::set_active_color(std::string const& fen, size_t begin_pos, size_t delim_pos) {
	// Is there a token?
	if (delim_pos == std::string::npos) { ERROR("space delimiter not found"); }

	// Is active color too long?
	if (delim_pos - begin_pos != 1) { ERROR("too long"); }

	// Is active color 'w' or 'b'?
	char color = fen[begin_pos];
	if (color != 'w' && color != 'b') { ERROR("must be 'w' or 'b'"); }

	this->active_color = color;
}

void FenClass::set_castling_rights(std::string const& fen, size_t begin_pos, size_t delim_pos) {
	// Is there a token?
	if (delim_pos == std::string::npos) { ERROR("space delimiter not found"); }

	// Are castling rights too long?
	size_t cr_size = delim_pos - begin_pos;
	if (cr_size > 4) { ERROR("too long"); }

	// Set castling_rights while validating substring contents

	char cr_c = fen[begin_pos];

	if (cr_size == 1) {
		if (cr_c == '-' || CR_LETTERS.contains(cr_c)) { this->castling_rights[0] = cr_c; }
		else { ERROR(std::format("invalid character '{}'", cr_c)); }
	} else {
		// Validate first character
		if (CR_LETTERS.contains(cr_c)) { this->castling_rights[0] = cr_c; }
		else { ERROR(std::format("invalid character '{}'", cr_c)); }

		// Loop remaining characters
		char last_c = cr_c;
		for (int cr_i = 1; cr_i < cr_size; cr_i++) {
			cr_c = fen[begin_pos + cr_i];
			if (CR_LETTERS.contains(cr_c)) {
				// Duplicates?
				if (cr_c == last_c) { ERROR(std::format("duplicate character '{}'", cr_c)); }

				// Sorted?
				else if (cr_c < last_c) { ERROR(std::format("wrong order for character '{}'", cr_c)); }

				// Put in array
				else { this->castling_rights[cr_i] = cr_c; }
			}
			else { ERROR(std::format("invalid character '{}'", cr_c)); }
		}
	}
	this->castling_rights[cr_size] = '\0';
}

void FenClass::set_en_passant_target(std::string const& fen, size_t begin_pos, size_t delim_pos) {
	// Is there a token?
	if (delim_pos == std::string::npos) { ERROR("space delimiter not found"); }

	// Is en passant target too long?
	size_t ept_size = delim_pos - begin_pos;
	if (ept_size > 2) { ERROR("too long"); }

	// Set en_passant_target while validating substring contents

	char ept_c = fen[begin_pos];

	if (ept_size == 2) {
		// File
		if (ept_c >= 'a' && ept_c <= 'h') { this->en_passant_target[0] = ept_c; }
		else { ERROR(std::format("invalid 1st character '{}', must be between 'a' and 'h'", ept_c)); }
		
		// Rank
		ept_c = fen[begin_pos+1];
		if (ept_c == '4' || ept_c == '5') { this->en_passant_target[1] = ept_c; }
		else { ERROR(std::format("invalid 2nd character '{}', must be '4' or '5'", ept_c)); }
	} else {
		// '-'
		if (ept_c == '-') { this->en_passant_target[0] = ept_c; }
		else { ERROR(std::format("invalid character '{}', must be '-' if one character", ept_c)); }
	}
	this->en_passant_target[ept_size] = '\0';
}

void FenClass::set_halfmove_clock(std::string const& fen, size_t begin_pos, size_t delim_pos) {
	// Is there a token?
	if (delim_pos == std::string::npos) { ERROR("space delimiter not found"); }

	// Is halfmove clock too long? MAX 100
	if (delim_pos - begin_pos > 3) { ERROR("too long"); }

	// Set halfmove_clock while validating substring contents

	size_t cur_pos = begin_pos;
	char hc_c;
	this->halfmove_clock = 0;

	while (cur_pos < delim_pos) {
		hc_c = fen[cur_pos++];
		if (std::isdigit(hc_c)) { this->halfmove_clock = this->halfmove_clock * 10 + (hc_c - '0'); }
		else { ERROR(std::format("non-digit character '{}'", hc_c)); }
	}
}

void FenClass::set_fullmove_number(std::string const& fen, size_t begin_pos) {
	// Is there a token?
	if (begin_pos >= fen.size()) { ERROR("no token found"); }

	// Is fullmove number too long? MAX 9999
	if (fen.size() - begin_pos > 4) { ERROR("too long"); }

	// Set fullmove_number while validating substring contents

	size_t cur_pos = begin_pos;
	char fn_c;
	this->fullmove_number = 0;

	while (cur_pos < fen.size()) {
		fn_c = fen[cur_pos++];
		if (std::isdigit(fn_c)) { this->fullmove_number = this->fullmove_number * 10 + (fn_c - '0'); }
		else { ERROR(std::format("non-digit character '{}'", fn_c)); }
	}

	if (this->fullmove_number < 1) { ERROR("less than 1"); }
}

/*
TODO: test more thoroughly
*/
FenClass::FenClass(std::string const& fen) {
	char delim = ' ';
	size_t begin_pos;
	size_t delim_pos;

	// Piece placement
	delim_pos = fen.find(delim, 0);
	this->set_piece_placement(fen, delim_pos);

	// Active color
	begin_pos = delim_pos + 1;
	delim_pos = fen.find(delim, begin_pos);
	this->set_active_color(fen, begin_pos, delim_pos);

	// Castling rights
	begin_pos = delim_pos + 1;
	delim_pos = fen.find(delim, begin_pos);
	this->set_castling_rights(fen, begin_pos, delim_pos);

	// En passant target
	begin_pos = delim_pos + 1;
	delim_pos = fen.find(delim, begin_pos);
	this->set_en_passant_target(fen, begin_pos, delim_pos);

	// Halfmove clock
	begin_pos = delim_pos + 1;
	delim_pos = fen.find(delim, begin_pos);
	this->set_halfmove_clock(fen, begin_pos, delim_pos);

	// Fullmove number
	begin_pos = delim_pos + 1;
	this->set_fullmove_number(fen, begin_pos);
}

/*
TODO: test
*/
std::string FenClass::to_string() {
	// Initialize string with sufficient reserved memory
	std::string fen;
	fen.reserve(FEN_BUF_SIZE);

	// Piece placement

	int empty_counter = 0;
    for (int pp_i = 0; pp_i < NUM_RANKS; pp_i++) {
        for (int pp_j = 0; pp_j < NUM_FILES; pp_j++) {
            char pp_c = this->piece_placement[pp_i][pp_j];
            if (pp_c == '\0') { empty_counter++; }
            else {
                // Record then reset empty counter if > 0
                if (empty_counter > 0) {
                	fen += empty_counter + '0';
                    empty_counter = 0;
                }
                // Record piece letter
                fen += pp_c;
            }
        }
        // Record then reset empty counter if > 0
        if (empty_counter > 0) {
            fen += empty_counter + '0';
            empty_counter = 0;
        }
        // Record a slash to end the row
        fen += '/';
    }
    // Replace last slash with a space
    fen.back() = ' ';

	// Active color
	fen += this->active_color;
	fen += ' ';

	// Castling rights
	fen += this->castling_rights;
	fen += ' ';

	// En passant target
	fen += this->en_passant_target;
	fen += ' ';

	// Halfmove clock
	fen += std::to_string(this->halfmove_clock);
	fen += ' ';

	// Fullmove number
	fen += std::to_string(this->fullmove_number);

	return fen;
}

void FenClass::print_piece_placement() {
	for (int i = 0; i < NUM_RANKS; i++) {
		for (int j = 0; j < NUM_FILES; j++) {
			std::print("{} ", this->piece_placement[i][j]);
		}
		std::println();
	}
}
