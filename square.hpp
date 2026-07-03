#pragma once

enum class File : unsigned char {
    A, // File a -> index j = 0
    B,
    C,
    D,
    E,
    F,
    G,
    H,
};

enum class Rank : unsigned char {
    R8, // Rank 8 -> index i = 0
    R7,
    R6,
    R5,
    R4,
    R3,
    R2,
    R1,
};

typedef struct {
    File file : 3;
    Rank rank : 3;
} Square;
