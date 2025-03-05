#include "types.h"

#include "constants.h"

namespace Chess::Util {

auto getPieceColor(Piece piece) -> Color
{
    if (piece == Piece::NONE) { return Color::NONE; }

    int color_value = static_cast<int>(piece) / Constants::PIECE_COLOR_OFFSET;
    return static_cast<Color>(color_value);
}

auto getPieceType(Piece piece) -> PieceType
{
    if (piece == Piece::NONE) { return PieceType::NONE; }

    int piece_value = static_cast<int>(piece) % Constants::PIECE_COLOR_OFFSET;
    return static_cast<PieceType>(piece_value);
}

auto makePiece(PieceType type, Color color) -> Piece
{
    if (type == PieceType::NONE) { return Piece::NONE; }
    if (color == Color::NONE) { return Piece::NONE; }

    int piece_value = static_cast<int>(type);
    if (color == Color::BLACK) { piece_value += Constants::PIECE_COLOR_OFFSET; }

    return static_cast<Piece>(piece_value);
}

auto getFile(Square square) -> int
{
    if (square == Square::NONE) { return Constants::NO_SQUARE; }
    return static_cast<int>(square) % Constants::BOARD_SIZE;
}

auto getRank(Square square) -> int
{
    if (square == Square::NONE) { return Constants::NO_SQUARE; }
    return static_cast<int>(square) / Constants::BOARD_SIZE;
}

auto makeSquare(int file, int rank) -> Square
{
    if (file < 0 || file > Constants::BOARD_SIZE - 1 || rank < 0 ||
        rank > Constants::BOARD_SIZE - 1) {
        return Square::NONE;
    }
    return static_cast<Square>((rank * Constants::BOARD_SIZE) + file);
}

} // namespace Chess::Util