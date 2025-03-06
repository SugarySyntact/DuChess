#include <iostream>

#include "bitboard.h"
#include "types.h"

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

    knight_moves |= northOne(northEastOne(squareBB(Square::E4))); // NNE
    knight_moves |= eastOne(northEastOne(squareBB(Square::E4)));  // ENE
    knight_moves |= eastOne(southEastOne(squareBB(Square::E4)));  // ESE
    knight_moves |= southOne(southEastOne(squareBB(Square::E4))); // SSE
    knight_moves |= southOne(southWestOne(squareBB(Square::E4))); // SSW
    knight_moves |= westOne(southWestOne(squareBB(Square::E4)));  // WSW
    knight_moves |= westOne(northWestOne(squareBB(Square::E4)));  // WNW
    knight_moves |= northOne(northWestOne(squareBB(Square::E4))); // NNW

    Bitboards::print(knight_moves);

    return 0;
}