#ifndef CHESS_BITBOARD_H
#define CHESS_BITBOARD_H

#include <array>

#include "constants.h"
#include "types.h"

namespace Chess {

class Bitboards {
public:
    static auto init() -> void;

    static auto lsb(Bitboard bitb) -> Square;
    static auto msb(Bitboard bitb) -> Square;
    static auto popCount(Bitboard bitb) -> int;
    static auto popLsb(Bitboard& bitb) -> Bitboard;

    static auto print(Bitboard bitb) -> void;

    static std::array<Bitboard, Constants::Board::LENGTH> files;
    static std::array<Bitboard, Constants::Board::LENGTH> ranks;
    static std::array<Bitboard, Constants::Board::DIAGONAL_COUNT> diagonals;
    static std::array<Bitboard, Constants::Board::DIAGONAL_COUNT> anti_diagonals;
    static std::array<Bitboard, Constants::Board::SQUARE_COUNT> squares;

private:
    static std::array<int, Constants::Board::SQUARE_COUNT> debruijn_lut;
    static constexpr Bitboard DEBRUIJN_CONSTANT = 0x03f79d71b4cb0a89ULL;
};

namespace Util {

inline auto testBit(Bitboard bitb, Square square) -> bool
{
    return (bitb & (1ULL << static_cast<unsigned>(square))) != 0;
};

inline auto setBit(Bitboard& bitb, Square square) -> void
{
    bitb |= (1ULL << static_cast<unsigned>(square));
}

inline auto clearBit(Bitboard& bitb, Square square) -> void
{
    bitb &= ~(1ULL << static_cast<unsigned>(square));
}

inline auto square(Square square) -> Bitboard { return 1ULL << static_cast<unsigned>(square); }

inline auto northOne(Bitboard bitb) -> Bitboard
{
    return bitb << static_cast<unsigned>(Constants::Board::LENGTH);
}

inline auto southOne(Bitboard bitb) -> Bitboard
{
    return bitb >> static_cast<unsigned>(Constants::Board::LENGTH);
}

inline auto eastOne(Bitboard bitb) -> Bitboard { return (bitb << 1ULL) & ~Bitboards::files.at(0); }

inline auto westOne(Bitboard bitb) -> Bitboard
{
    return (bitb >> 1ULL) & ~Bitboards::files.at(Constants::Board::LENGTH - 1);
}

inline auto northEastOne(Bitboard bitb) -> Bitboard { return northOne(eastOne(bitb)); }
inline auto nothWestOne(Bitboard bitb) -> Bitboard { return northOne(westOne(bitb)); }
inline auto southEastOne(Bitboard bitb) -> Bitboard { return southOne(eastOne(bitb)); }
inline auto southWestOne(Bitboard bitb) -> Bitboard { return southOne(westOne(bitb)); }

} // namespace Util

} // namespace Chess

#endif // CHESS_BITBOARD_H