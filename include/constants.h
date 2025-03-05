#ifndef CHESS_CONSTANTS_H
#define CHESS_CONSTANTS_H

namespace Chess::Constants {

constexpr int PIECE_COLOR_OFFSET = 8;

constexpr unsigned int DEBRUIJN_SHIFT = 58;
constexpr unsigned int MSB_RSHIFT_COUNT = 6;

constexpr int BASE_16_HEX = 16;

namespace Board {

constexpr int LENGTH = 8;
constexpr int SQUARE_COUNT = LENGTH * LENGTH;
constexpr int DIAGONAL_COUNT = (2 * LENGTH) - 1;
constexpr int DIAGONAL_CENTER = LENGTH - 1;

constexpr int NO_SQUARE = -1;

} // namespace Board

} // namespace Chess::Constants

#endif // CHESS_CONSTANTS_H