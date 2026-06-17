#include <cctype>
#include <cstdlib>
#include <cstring>
#include <format>
#include <print>
#include <string>
#include "fen.hpp"

#define NUM_FILES 8
#define NUM_RANKS 8

const std::unordered_set<char> piece_letters{'K', 'Q', 'R', 'B', 'N', 'P', 'k', 'q', 'r', 'b', 'n', 'p'};

FenClass::FenClass(char *fen_s) {
    /*
    Validates the contents of parameter FEN string and sets their values to the class members.
    */

    // Lambda expression to print errors and exit
    auto bad_tok = [](std::string name, const char *reason) {
        std::println("Invalid {} for FEN string! Reason: {}", name, reason);
        exit(1);
    };

    // Declaring vars for std::strtok and error handling
    std::string name;
    const char *delim = " ";
    char *token;

    /* PIECE PLACEMENT */
    
    name = "piece placement";
    token = std::strtok(fen_s, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Is piece placement the right size?
    size_t tok_len = std::strlen(token);
    size_t min_len = 17; // Length of minimum-size piece placement: 8/8/8/8/8/8/k7/K7
    size_t max_len = 71; // 64 piece letters + 7 '/' characters
    if (tok_len < min_len) bad_tok(name, "too short");
    else if (tok_len > max_len) bad_tok(name, "too long");

    // Set piece_placement while validating token contents
    int i = 0; // rank index
    int j = 0; // file index
    char *p;
    for (p = token; *p != '\0'; p += sizeof(char)) {
        // Piece letter
        if (piece_letters.contains(*p)) {
            // j must be within array bounds
            if (j > NUM_FILES - 1) bad_tok(name, std::format("too many files on rank {}", 8 - i).c_str());
            // Put letter in piece_placements and increment j
            this->piece_placement[i][j++] = *p;
        // Number
        } else if (std::isdigit(static_cast<int>(*p))) {
            // Add to j by number
            j += *p - '0';
            // j must be <= NUM_FILES after adding
            if (j > NUM_FILES) bad_tok(name, std::format("too many files on rank {}", 8 - i).c_str());
        // '/' end of rank
        } else if (*p == '/') {
            // j must be = NUM_FILES
            if (j < NUM_FILES) bad_tok(name, std::format("too few files on rank {}", 8 - i).c_str());
            // Increment i, must be within array bounds
            if (++i > NUM_RANKS - 1) bad_tok(name, "too many ranks, i.e. '/' characters");
            // Reset j to begin next rank
            j = 0;
        // Anything else is invalid
        } else bad_tok(name, std::format("invalid character '{}'", *p).c_str());
    }
    // Check if full piece placement token was given
    if (i < NUM_RANKS - 1) bad_tok(name, "too few ranks");
    else if (j < NUM_FILES) bad_tok(name, "too few files on rank 1");

    /* ACTIVE COLOR */
    
    name = "active color";
    token = std::strtok(nullptr, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Is active color too long?
    if (std::strlen(token) > 1) bad_tok(name, "too long");

    // Is active color 'w' or 'b'?
    else if (*token != 'w' && *token != 'b') bad_tok(name, "must be 'w' or 'b'");

    // Set active_color
    this->active_color = *token;

    /* CASTLING RIGHTS */
    
    name = "castling rights";
    token = std::strtok(nullptr, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Are castling rights too long?
    tok_len = std::strlen(token);
    if (tok_len > 4) bad_tok(name, "too long");

    // Set castling_rights while validating token contents
    std::unordered_set<char> cr_letters{'K', 'Q', 'k', 'q'};
    if (tok_len == 1) {
        // '-' or in cr_letters?
        if (*token != '-' && !cr_letters.contains(*token)) bad_tok(name, std::format("invalid character '{}'", *p).c_str());
        else {
            this->castling_rights[0] = *token;
            this->castling_rights[1] = '\0';
        }
    } else {
        // Check first character in cr_letters, then store it
        if (!cr_letters.contains(*token)) bad_tok(name, std::format("invalid character '{}'", *token).c_str());
        else this->castling_rights[0] = *token;
        char last_c = *token;
        for (p = token + sizeof(char); *p != '\0'; p += sizeof(char)) {
            // Check character in cr_letters
            if (!cr_letters.contains(*p)) bad_tok(name, std::format("invalid character '{}'", *p).c_str());
            // Are there duplicates?
            else if (*p == last_c) bad_tok(name, std::format("duplicate character '{}'", *p).c_str());
            // Are castling rights sorted?
            else if (*p < last_c) bad_tok(name, std::format("wrong order for character '{}'", *p).c_str());
            // Set current castling_rights index
            else this->castling_rights[(p - token) / sizeof(char)] = *p;
            // Update last_c
            last_c = *p;
        }
        // Don't forget to set null terminator!
        this->castling_rights[(p - token) / sizeof(char)] = *p;
    }

    /* EN PASSANT TARGET */

    name = "en passant target";
    token = std::strtok(nullptr, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Is en passant target too long?
    tok_len = std::strlen(token);
    if (tok_len > 2) bad_tok(name, "too long");

    // Is en passant target a hyphen or valid square?
    if (tok_len == 1) {
        if (*token != '-') bad_tok(name, "one-length string must be '-'");
    } else if (token[0] < 'a' || token[0] > 'h' || (token[1] != '4' && token[1] != '5')) bad_tok(name, "two-length string must be valid square, e.g. \"e4\"");

    // Set en_passant_target
    std::strcpy(this->en_passant_target, token);

    /* HALFMOVE CLOCK */

    name = "halfmove clock";
    token = std::strtok(nullptr, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Is halfmove clock an integer?
    char *end;
    long halfmove_clock = std::strtol(token, &end, 10);
    if (end == token) bad_tok(name, "can't convert string to integer");
    else if (*end != '\0') bad_tok(name, std::format("non-integer character '{}'", *end).c_str());

    // Is halfmove clock between 0 and 100?
    if (halfmove_clock < 0 || halfmove_clock > 100) bad_tok(name, "must be between 0 and 100 inclusive");

    // Set halfmove_clock
    this->halfmove_clock = halfmove_clock;

    /* FULLMOVE NUMBER */

    name = "fullmove number";
    token = std::strtok(nullptr, delim);

    // Is there a token?
    if (token == nullptr) bad_tok(name, "nullptr");

    // Is fullmove number an integer?
    long fullmove_number = std::strtol(token, &end, 10);
    if (end == token) bad_tok(name, "can't convert string to integer");
    else if (*end != '\0') bad_tok(name, std::format("non-integer character '{}'", *end).c_str());

    // Is fullmove number at least 1?
    if (fullmove_number < 1) bad_tok(name, "must be at least 1");

    // Set fullmove_number
    this->fullmove_number = fullmove_number;
}

void FenClass::to_string(char *dest_buf, size_t buf_size) {
    int buf_i = 0;

    /* PIECE PLACEMENT */

    int empty_counter = 0;
    for (int pp_i = 0; pp_i < NUM_RANKS; pp_i++) {
        for (int pp_j = 0; pp_j < NUM_FILES; pp_j++) {
            char pp_c = this->piece_placement[pp_i][pp_j];
            if (pp_c == '\0') empty_counter++;
            else {
                // Record then reset empty counter if > 0
                if (empty_counter > 0) {
                    dest_buf[buf_i++] = empty_counter + '0';
                    empty_counter = 0;
                }
                // Record piece letter
                dest_buf[buf_i++] = pp_c;
            }
        }
        // Record then reset empty counter if > 0
        if (empty_counter > 0) {
            dest_buf[buf_i++] = empty_counter + '0';
            empty_counter = 0;
        }
        // Record a slash to end the row
        dest_buf[buf_i++] = '/';
    }
    // Replace last slash with a space
    dest_buf[buf_i - 1] = ' ';

    /* ACTIVE COLOR */

    dest_buf[buf_i++] = this->active_color;
    dest_buf[buf_i++] = ' ';

    /* CASTLING RIGHTS */

    for (int cr_i = 0; this->castling_rights[cr_i] != '\0'; cr_i++) {
        dest_buf[buf_i++] = this->castling_rights[cr_i];
    }
    dest_buf[buf_i++] = ' ';

    /* EN PASSANT TARGET */

    for (int ept_i = 0; this->en_passant_target[ept_i] != '\0'; ept_i++) {
        dest_buf[buf_i++] = this->en_passant_target[ept_i];
    }
    dest_buf[buf_i++] = ' ';

    /* HALFMOVE CLOCK */

    // TODO: complete halfmove clock and fullmove number
}
