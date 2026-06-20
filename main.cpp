#include <cstring>
#include <iostream>
#include <print>
#include <random>
#include "color.hpp"
#include "fen.hpp"

int main(int argc, char* argv[]) {
    // Get FEN string
    char fen_s[FEN_BUF_SIZE];
    if (argc > 1) {
        if (std::strlen(argv[1]) < FEN_BUF_SIZE) std::strcpy(fen_s, argv[1]);
        else {
            std::println("FEN string too large!");
            exit(1);
        }
    } else std::strcpy(fen_s, START_FEN);

    FenClass fen_class(fen_s);
    char fen_buf[FEN_BUF_SIZE];
    fen_class.to_string(fen_buf, FEN_BUF_SIZE);
    std::println("fen_buf = {}\nlength = {}\nSTART_FEN length = {}", static_cast<char*>(fen_buf), std::strlen(fen_buf), std::strlen(START_FEN));

    return 0;

    // Get user color
    std::print("Choose color (0: White, 1: Black, _: Random): ");
    int i;
    std::cin >> i;
    Color user_color = static_cast<Color>(i);
    if (user_color != Color::White && user_color != Color::Black) {
        std::random_device rd; // Random seed
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<int> distrib(0, 1); // Range
        user_color = static_cast<Color>(distrib(gen));
    }

    return 0;
}
