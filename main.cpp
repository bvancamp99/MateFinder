#include <cstring>
#include <iostream>
#include <print>
#include <random>
#include "color.hpp"
#include "fen.hpp"

int main(int argc, char* argv[]) {
    // Get FEN string
    std::string fen_s;
    if (argc > 1) {
        if (std::strlen(argv[1]) < FEN_BUF_SIZE) { fen_s = argv[1]; }
        else {
            std::println("FEN string too large!");
            exit(1);
        }
    } else { fen_s = START_FEN; }

    FenClass fen_class(fen_s);

    return 0;

    // Get user color
    std::print("Choose color (0: White, 1: Black, _: Random): ");
    int i;
    std::cin >> i;
    Color user_color = static_cast<Color>(i);
    if (user_color != Color::White && user_color != Color::Black) {
        // Mersenne Twister engine with random seed
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 1);
        user_color = static_cast<Color>(dist(rng));
    }

    return 0;
}
