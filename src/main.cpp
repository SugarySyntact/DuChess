#include <iostream>

#include "bitboard.h"
#include "position.h"
#include "zobrist.h"

using namespace Chess;
using namespace Util;

auto main() -> int
{
    Bitboards::init();
    Zobrist::init();

    std::cout << "\n╔══════════════════════════════════════╗";
    std::cout << "\n║            DuChess Engine            ║";
    std::cout << "\n╚══════════════════════════════════════╝\n";

    Position position;

    position.print();

    return 0;
}