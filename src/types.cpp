#include "types.h"

#include <cctype>
#include <stdexcept>

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

auto squareToString(Square square) -> std::string
{
    if (square == Square::NONE) { return "-"; }

    const int SQR = static_cast<int>(square);
    const int FILE = SQR % Constants::Board::LENGTH;
    const int RANK = SQR / Constants::Board::LENGTH;

    const char FILE_CHAR = static_cast<char>('a' + FILE);
    const char RANK_CHAR = static_cast<char>('1' + RANK);

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

    const int FILE = FILE_CHAR - 'a';
    const int RANK = RANK_CHAR - '1';

    return makeSquare(FILE, RANK);
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

auto pieceToChar(Piece piece) -> char
{
    switch (piece) {
        case Piece::NONE: return '.';
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
        default: return '?';
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