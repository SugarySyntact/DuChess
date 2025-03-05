#include <iostream>

#include "bitboard.h"

using namespace Chess;
using namespace Util;

auto main() -> int
{
    Bitboards::init();

    std::cout << "\n╔══════════════════════════════════════╗";
    std::cout << "\n║            DuChess Engine            ║";
    std::cout << "\n╚══════════════════════════════════════╝\n";

    std::cout << "\nKnight's moves from E4\n";
    Bitboard knight_moves = 0;

    knight_moves |= northOne(northEastOne(square(Square::E4))); // NNE
    knight_moves |= eastOne(northEastOne(square(Square::E4)));  // ENE
    knight_moves |= eastOne(southEastOne(square(Square::E4)));  // ESE
    knight_moves |= southOne(southEastOne(square(Square::E4))); // SSE
    knight_moves |= southOne(southWestOne(square(Square::E4))); // SSW
    knight_moves |= westOne(southWestOne(square(Square::E4)));  // WSW
    knight_moves |= westOne(northWestOne(square(Square::E4)));  // WNW
    knight_moves |= northOne(northWestOne(square(Square::E4))); // NNW

    Bitboards::print(knight_moves);

    return 0;
}