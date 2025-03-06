#include "position.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "zobrist.h"

namespace Chess {

using namespace Util;

Position::Position() : Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

Position::Position(const std::string& fen)
    : m_piece_bitboards(), m_color_bitboards(), m_pieces(), m_castling_rights(),
      m_position_hash(computeHash())
{
    std::istringstream iss(fen);
    std::string token;

    // 1. Piece placement
    iss >> token;
    int file = 0;
    int rank = Constants::Board::LENGTH - 1; // FEN starts from the 8th rank

    for (char chr : token) {
        if (chr == '/') {
            file = 0;
            --rank;
        }
        else if (chr >= '1' && chr <= '8') {
            file += chr - '0';
        }
        else {
            Piece piece = charToPiece(chr);
            Square square = makeSquare(file, rank);
            m_pieces.at(static_cast<int>(square)) = piece;

            // Update bitboards
            Color color = getPieceColor(piece);
            PieceType type = getPieceType(piece);
            Bitboard square_bit = squareBB(square);
            m_piece_bitboards.at(static_cast<int>(color)).at(static_cast<int>(type) - 1) |=
                square_bit;
            m_color_bitboards.at(static_cast<int>(color)) |= square_bit;

            ++file;
        }
    }

    // 2. Active color
    iss >> token;
    m_side_to_move = (token == "w") ? Color::WHITE : Color::BLACK;

    // 3. Castling availability
    iss >> token;
    for (char chr : token) {
        switch (chr) {
            case 'K':
                m_castling_rights |= static_cast<uint8_t>(CastlingRight::WHITE_KINGSIDE);
                break;
            case 'Q':
                m_castling_rights |= static_cast<uint8_t>(CastlingRight::WHITE_QUEENSIDE);
                break;
            case 'k':
                m_castling_rights |= static_cast<uint8_t>(CastlingRight::BLACK_KINGSIDE);
                break;
            case 'q':
                m_castling_rights |= static_cast<uint8_t>(CastlingRight::BLACK_QUEENSIDE);
                break;
            case '-':
                // No castling rights
                break;
            default: throw std::invalid_argument("Invalid castling rights in FEN: " + token);
        }
    }

    // 4. En passant target square
    iss >> token;
    m_en_passant_square = (token == "-") ? Square::NONE : stringToSquare(token);

    // 5. Halfmove clock
    iss >> token;
    m_halfmove_clock = std::stoi(token);

    // 6. Fullmove number
    iss >> token;
    m_fullmove_number = std::stoi(token);
}

auto Position::pieceAt(Square square) const -> Piece
{
    if (square == Square::NONE) { return Piece::NONE; }
    return m_pieces.at(static_cast<int>(square));
}

auto Position::getPieceBitboard(PieceType type, Color color) const -> Bitboard
{
    if (type == PieceType::NONE || color == Color::NONE) { return 0; }
    return m_piece_bitboards.at(static_cast<int>(color)).at(static_cast<int>(type) - 1);
}

auto Position::getColorBitboard(Color color) const -> Bitboard
{
    if (color == Color::NONE) { return 0; }
    return m_color_bitboards.at(static_cast<int>(color));
}

auto Position::getOccupiedBitboard() const -> Bitboard
{
    return m_color_bitboards[static_cast<int>(Color::WHITE)] |
           m_color_bitboards[static_cast<int>(Color::BLACK)];
}

auto Position::getSideToMove() const -> Color { return m_side_to_move; }

auto Position::hasCastlingRight(CastlingRight right) const -> bool
{
    return (m_castling_rights & static_cast<uint8_t>(right)) != 0;
}

auto Position::getCastlingRights() const -> CastlingRightsBitField { return m_castling_rights; }

auto Position::getEnPassantSquare() const -> Square { return m_en_passant_square; }

auto Position::getHalfmoveClock() const -> int { return m_halfmove_clock; }

auto Position::getFullmoveNumber() const -> int { return m_fullmove_number; }

auto Position::hash() const -> HashKey { return m_position_hash; }

auto Position::toFen() const -> std::string
{
    std::ostringstream oss;

    // 1. Piece placement
    for (int rank = Constants::Board::LENGTH - 1; rank >= 0; --rank) {
        int empty_count = 0;
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            Square sqr = makeSquare(file, rank);
            Piece piece = pieceAt(sqr);

            if (piece == Piece::NONE) {
                ++empty_count;
                continue;
            }
            if (empty_count > 0) {
                oss << empty_count;
                empty_count = 0;
            }
            oss << pieceToChar(piece);
        }

        if (empty_count > 0) { oss << empty_count; }

        if (rank > 0) { oss << '/'; }
    }

    // 2. Active color
    oss << ' ' << (getSideToMove() == Color::WHITE ? 'w' : 'b');

    // 3. Castling availability
    oss << ' ';
    if (getCastlingRights() == 0) { oss << '-'; }
    else {
        if (hasCastlingRight(CastlingRight::WHITE_KINGSIDE)) { oss << 'K'; }
        if (hasCastlingRight(CastlingRight::WHITE_QUEENSIDE)) { oss << 'Q'; }
        if (hasCastlingRight(CastlingRight::BLACK_KINGSIDE)) { oss << 'k'; }
        if (hasCastlingRight(CastlingRight::BLACK_QUEENSIDE)) { oss << 'q'; }
    }

    // 4. En passant target square
    oss << ' ' << squareToString(getEnPassantSquare());

    // 5. Halfmove clock
    oss << ' ' << getHalfmoveClock();

    // 6. Fullmove number
    oss << ' ' << getFullmoveNumber();

    return oss.str();
}

auto Position::print() const -> void
{
    std::cout << "+---+---+---+---+---+---+---+---+\n";
    for (int rank = Constants::Board::LENGTH - 1; rank >= 0; --rank) {
        std::cout << "| ";
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            Square sqr = makeSquare(file, rank);
            Piece piece = pieceAt(sqr);
            std::cout << pieceToChar(piece) << " | ";
        }
        std::cout << (rank + 1) << "\n";
        std::cout << "+---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "  a   b   c   d   e   f   g   h\n";

    // Print additional information
    std::cout << "Side to move: " << (getSideToMove() == Color::WHITE ? "White" : "Black") << "\n";
    std::cout << "Castling rights: ";
    if (hasCastlingRight(CastlingRight::WHITE_KINGSIDE)) { std::cout << "K"; }
    if (hasCastlingRight(CastlingRight::WHITE_QUEENSIDE)) { std::cout << "Q"; }
    if (hasCastlingRight(CastlingRight::BLACK_KINGSIDE)) { std::cout << "k"; }
    if (hasCastlingRight(CastlingRight::BLACK_QUEENSIDE)) { std::cout << "q"; }
    if (getCastlingRights() == 0) { std::cout << "-"; }
    std::cout << "\n";

    std::cout << "En passant square: " << squareToString(getEnPassantSquare()) << "\n";
    std::cout << "Halfmove clock: " << getHalfmoveClock() << "\n";
    std::cout << "Fullmove number: " << getFullmoveNumber() << "\n";
    std::cout << "Position hash: 0x" << std::hex << hash() << std::dec << "\n";
}

auto Position::operator==(const Position& other) const -> bool
{
    // Quick check using hash
    if (hash() != other.hash()) { return false; }

    // Full equality check
    for (int i = 0; i < Constants::Board::SQUARE_COUNT; ++i) {
        if (m_pieces.at(i) != other.m_pieces.at(i)) { return false; }
    }

    return m_side_to_move == other.m_side_to_move && /* clang-format */
           m_castling_rights == other.m_castling_rights &&
           m_en_passant_square == other.m_en_passant_square &&
           m_halfmove_clock == other.m_halfmove_clock &&
           m_fullmove_number == other.m_fullmove_number;
}

auto Position::operator!=(const Position& other) const -> bool { return !(*this == other); }

auto Position::computeHash() const -> HashKey
{
    HashKey hash = 0;

    // 1. Pieces
    for (int square = 0; square < Constants::Board::SQUARE_COUNT; ++square) {
        Piece piece = m_pieces.at(square);
        if (piece != Piece::NONE) {
            hash ^= Zobrist::getPieceSquareKey(piece, static_cast<Square>(square));
        }
    }

    // 2. Side to move
    if (m_side_to_move == Color::BLACK) { hash ^= Zobrist::getSideToMoveKey(); }

    // 3. Castling rights
    hash ^= Zobrist::getCastlingKey(m_castling_rights);

    // 4. En passant square
    hash ^= Zobrist::getEnPassantKey(m_en_passant_square);

    return hash;
}

} // namespace Chess