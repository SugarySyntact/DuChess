#include "types.h"

#include <cctype>
#include <stdexcept>

#include "constants.h"

namespace Chess::Util {

auto getPieceColor(Piece piece) -> Color
{
    if (piece == Piece::NONE) { return Color::NONE; }
    return fromIdx<Color>(toIdx(piece) / Constants::PIECE_COLOR_OFFSET);
}

auto getPieceType(Piece piece) -> PieceType
{
    if (piece == Piece::NONE) { return PieceType::NONE; }
    return fromIdx<PieceType>(toIdx(piece) % Constants::PIECE_COLOR_OFFSET);
}

auto makePiece(PieceType type, Color color) -> Piece
{
    if (type == PieceType::NONE) { return Piece::NONE; }
    if (color == Color::NONE) { return Piece::NONE; }

    return fromIdx<Piece>(toIdx(type) + (toIdx(color) * Constants::PIECE_COLOR_OFFSET));
}

auto squareToString(Square square) -> std::string
{
    if (square == Square::NONE) { return "-"; }

    const char FILE_CHAR = static_cast<char>('a' + getFile(square));
    const char RANK_CHAR = static_cast<char>('1' + getRank(square));

    return std::string(1, FILE_CHAR) + std::string(1, RANK_CHAR);
}

auto stringToSquare(const std::string& str) -> Square
{
    if (str == "-") { return Square::NONE; }
    if (str.size() != 2) { throw std::invalid_argument("Invalid square string: " + str); }

    const char FILE_CHAR = static_cast<char>(std::tolower(str[0]));
    const char RANK_CHAR = str[1];

    if (FILE_CHAR < 'a' || FILE_CHAR > 'h' || RANK_CHAR < '1' || RANK_CHAR > '8') {
        throw std::invalid_argument("Invalid square string: " + str);
    }

    return makeSquare(FILE_CHAR - 'a', RANK_CHAR - '1');
}

auto getFile(Square square) -> int
{
    if (square == Square::NONE) { return Constants::Board::NO_SQUARE; }
    return toIdx(square) % Constants::Board::LENGTH;
}

auto getRank(Square square) -> int
{
    if (square == Square::NONE) { return Constants::Board::NO_SQUARE; }
    return toIdx(square) / Constants::Board::LENGTH;
}

auto makeSquare(int file, int rank) -> Square
{
    if (file < 0 || file > Constants::Board::LENGTH - 1 || rank < 0 ||
        rank > Constants::Board::LENGTH - 1) {
        return Square::NONE;
    }
    return fromIdx<Square>((rank * Constants::Board::LENGTH) + file);
}

auto pieceToChar(Piece piece) -> char
{
    switch (piece) {
        case Piece::WHITE_PAWN: return 'P';
        case Piece::WHITE_KNIGHT: return 'N';
        case Piece::WHITE_BISHOP: return 'B';
        case Piece::WHITE_ROOK: return 'R';
        case Piece::WHITE_QUEEN: return 'Q';
        case Piece::WHITE_KING: return 'K';
        case Piece::BLACK_PAWN: return 'p';
        case Piece::BLACK_KNIGHT: return 'n';
        case Piece::BLACK_BISHOP: return 'b';
        case Piece::BLACK_ROOK: return 'r';
        case Piece::BLACK_QUEEN: return 'q';
        case Piece::BLACK_KING: return 'k';
        case Piece::NONE:
        default: return '.';
    }
}

auto charToPiece(char chr) -> Piece
{
    switch (chr) {
        case '.': return Piece::NONE;
        case 'P': return Piece::WHITE_PAWN;
        case 'N': return Piece::WHITE_KNIGHT;
        case 'B': return Piece::WHITE_BISHOP;
        case 'R': return Piece::WHITE_ROOK;
        case 'Q': return Piece::WHITE_QUEEN;
        case 'K': return Piece::WHITE_KING;
        case 'p': return Piece::BLACK_PAWN;
        case 'n': return Piece::BLACK_KNIGHT;
        case 'b': return Piece::BLACK_BISHOP;
        case 'r': return Piece::BLACK_ROOK;
        case 'q': return Piece::BLACK_QUEEN;
        case 'k': return Piece::BLACK_KING;
        default: return Piece::NONE;
    }
}

} // namespace Chess::Util