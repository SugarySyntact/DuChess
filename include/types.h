#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <cstdint>

namespace Chess {

enum class PieceType : uint8_t { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum class Color : uint8_t { WHITE, BLACK, NONE };

// clang-format off
enum class Piece : uint8_t {
    NONE = 0,
    WHITE_PAWN = 1,     BLACK_PAWN = 9,
    WHITE_KNIGHT = 2,   BLACK_KNIGHT = 10,
    WHITE_BISHOP = 3,   BLACK_BISHOP = 11,
    WHITE_ROOK = 4,     BLACK_ROOK = 12,
    WHITE_QUEEN = 5,    BLACK_QUEEN = 13,
    WHITE_KING = 6,     BLACK_KING = 14
};

enum class Square : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NONE
};
// clang-format on

namespace Util {

auto getPieceColor(Piece piece) -> Color;
auto getPieceType(Piece piece) -> PieceType;
auto makePiece(PieceType type, Color color) -> Piece;

auto getFile(Square square) -> int;
auto getRank(Square square) -> int;
auto makeSquare(int file, int rank) -> Square;

} // namespace Util

} // namespace Chess

#endif // CHESS_TYPES_H