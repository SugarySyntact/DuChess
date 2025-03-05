#include "types.h"

#include "constants.h"

namespace Chess::Util {

auto getPieceColor(Piece piece) -> Color
{
    if (piece == Piece::NONE) { return Color::NONE; }

    const int COLOR_VALUE = static_cast<int>(piece) / Constants::PIECE_COLOR_OFFSET;
    return static_cast<Color>(COLOR_VALUE);
}

auto getPieceType(Piece piece) -> PieceType
{
    if (piece == Piece::NONE) { return PieceType::NONE; }

    const int PIECE_VALUE = static_cast<int>(piece) % Constants::PIECE_COLOR_OFFSET;
    return static_cast<PieceType>(PIECE_VALUE);
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
    if (square == Square::NONE) { return Constants::Board::NO_SQUARE; }
    return static_cast<int>(square) % Constants::Board::LENGTH;
}

auto getRank(Square square) -> int
{
    if (square == Square::NONE) { return Constants::Board::NO_SQUARE; }
    return static_cast<int>(square) / Constants::Board::LENGTH;
}

auto makeSquare(int file, int rank) -> Square
{
    if (file < 0 || file > Constants::Board::LENGTH - 1 || rank < 0 ||
        rank > Constants::Board::LENGTH - 1) {
        return Square::NONE;
    }
    return static_cast<Square>((rank * Constants::Board::LENGTH) + file);
}

} // namespace Chess::Util