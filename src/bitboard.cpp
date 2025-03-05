#include "bitboard.h"

#include <iomanip>
#include <iostream>

#include "compiler_macros.h"
#include "constants.h"
#include "types.h"

namespace Chess {

using namespace Util;

auto Bitboards::init() -> void
{
    for (int file = 0; file < Constants::Board::LENGTH; ++file) {
        files.at(file) = 0ULL;
        UNROLL_LOOP
        for (int rank = 0; rank < Constants::Board::LENGTH; ++rank) {
            setBit(files.at(file), makeSquare(file, rank));
        }
    }

    for (int rank = 0; rank < Constants::Board::LENGTH; ++rank) {
        ranks.at(rank) = 0ULL;
        UNROLL_LOOP
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            setBit(ranks.at(rank), makeSquare(file, rank));
        }
    }

    for (int i = 0; i < Constants::Board::DIAGONAL_COUNT; ++i) {
        diagonals.at(i) = 0ULL;
        int diag = i - Constants::Board::DIAGONAL_CENTER;
        UNROLL_LOOP
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            const int RANK = file - diag;
            if (RANK >= 0 && RANK < Constants::Board::LENGTH) {
                setBit(diagonals.at(i), makeSquare(file, RANK));
            }
        }
    }

    for (int i = 0; i < Constants::Board::DIAGONAL_COUNT; ++i) {
        anti_diagonals.at(i) = 0ULL;
        const int DIAG = i - Constants::Board::DIAGONAL_CENTER;
        UNROLL_LOOP
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            const int RANK = Constants::Board::DIAGONAL_CENTER - file - DIAG;
            if (RANK >= 0 && RANK < Constants::Board::LENGTH) {
                setBit(anti_diagonals.at(i), makeSquare(file, RANK));
            }
        }
    }

    UNROLL_LOOP
    for (unsigned sq = 0; sq < Constants::Board::SQUARE_COUNT; ++sq) {
        squares.at(sq) = 1ULL << sq;
    }

    // clang-format off
    constexpr std::array<int, Constants::Board::SQUARE_COUNT> INDEX64 = {
        0,  1,  48, 2,  57, 49, 28, 3,
        61, 58, 50, 42, 38, 29, 4,  17,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 5,  18, 11,
        63, 10, 56, 21, 27, 37, 47, 35,
        54, 52, 15, 32, 23, 14, 44, 20,
        46, 26, 34, 25, 60, 19, 13, 9, 
        12, 8,  16, 7,  41, 40, 6,  31
    };
    // clang-format on
    debruijn_lut = INDEX64;
}

auto Bitboards::lsb(Bitboard bitb) -> Square
{
    if (bitb == 0) { return Square::NONE; }
    return static_cast<Square>(
        debruijn_lut.at(((bitb & -bitb) * DEBRUIJN_CONSTANT) >> Constants::DEBRUIJN_SHIFT));
}

auto Bitboards::msb(Bitboard bitb) -> Square
{
    if (bitb == 0) { return Square::NONE; }

    UNROLL_LOOP
    for (unsigned int i = 0; i < Constants::MSB_RSHIFT_COUNT; ++i) { bitb |= bitb >> (1U << i); }

    return static_cast<Square>(
        debruijn_lut.at((bitb * DEBRUIJN_CONSTANT) >> Constants::DEBRUIJN_SHIFT));
}

auto Bitboards::popCount(Bitboard bitb) -> int
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(bitb);
#else
    int count = 0;
    while (b) {
        b &= b - 1;
        count++;
    }
    return count;
#endif
}

auto Bitboards::popLsb(Bitboard& bitb) -> Bitboard
{
    const Bitboard BITB = bitb;
    bitb &= (bitb - 1);
    return BITB & ~bitb;
}

void Bitboards::print(Bitboard bitb)
{
    std::cout << "+---+---+---+---+---+---+---+---+\n";
    for (int rank = Constants::Board::LENGTH - 1; rank >= 0; --rank) {
        std::cout << "| ";
        UNROLL_LOOP
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            const Square SQUARE = makeSquare(file, rank);
            if (testBit(bitb, SQUARE)) { std::cout << "X | "; }
            else {
                std::cout << "  | ";
            }
        }
        std::cout << rank + 1 << "\n";
        std::cout << "+---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "  a   b   c   d   e   f   g   h\n";
    std::cout << "Hex: 0x" << std::hex << std::setw(Constants::BASE_16_HEX) << std::setfill('0')
              << bitb << std::dec << "\n";
}

} // namespace Chess