#include "position.h"

#include <cstdint>
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
    : m_piece_bitboards(), m_color_bitboards(), m_pieces(), m_side_to_move(Color::WHITE),
      m_castling_rights(0), m_en_passant_square(Square::NONE), m_halfmove_clock(0),
      m_fullmove_number(1)
{
    std::istringstream iss(fen);

    parseFenPiecePlacement(iss);
    parseFenGameState(iss);

    // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer) - Must be init last
    m_position_hash = computeHash();
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

    oss << buildFenPiecePlacement();
    oss << buildFenGameState();

    return oss.str();
}

auto Position::print() const -> void
{
    std::cout << "+---+---+---+---+---+---+---+---+\n";

    // Use reverse rank (r_rank) to silence `-altera-id-dependent-backward-branch`
    for (int r_rank = 0; r_rank < Constants::Board::LENGTH; ++r_rank) {
        const int RANK = Constants::Board::MAX_RANK - r_rank;
        std::cout << "| ";
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            const Square SQR = makeSquare(file, RANK);
            const Piece PIECE = pieceAt(SQR);
            std::cout << pieceToChar(PIECE) << " | ";
        }
        std::cout << (RANK + 1) << "\n";
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
           m_en_passant_square == other.m_en_passant_square;
}

auto Position::operator!=(const Position& other) const -> bool { return !(*this == other); }

auto Position::parseFenPiecePlacement(std::istringstream& iss) -> void
{
    std::string token;
    if (!(iss >> token)) { return; }
    int file = 0;
    int rank = Constants::Board::LENGTH - 1; // FEN starts from the 8th rank

    for (const char CHR : token) {
        if (CHR == '/') {
            file = 0;
            --rank;
        }
        else if (CHR >= '1' && CHR <= '8') {
            file += CHR - '0';
        }
        else {
            const Piece PIECE = charToPiece(CHR);
            const Square SQUARE = makeSquare(file, rank);

            // Add a bounds check to ensure square is valid
            if (SQUARE != Square::NONE &&
                static_cast<int>(SQUARE) < Constants::Board::SQUARE_COUNT) {
                m_pieces.at(static_cast<int>(SQUARE)) = PIECE;

                // Update bitboards
                const Color COLOR = getPieceColor(PIECE);
                const PieceType TYPE = getPieceType(PIECE);

                if (COLOR != Color::NONE && TYPE != PieceType::NONE) {
                    const Bitboard SQUARE_BIT = squareBB(SQUARE);
                    m_piece_bitboards.at(static_cast<int>(COLOR)).at(static_cast<int>(TYPE) - 1) |=
                        SQUARE_BIT;
                    m_color_bitboards.at(static_cast<int>(COLOR)) |= SQUARE_BIT;
                }
            }
            ++file;
        }
    }
}

auto Position::parseFenGameState(std::istringstream& iss) -> void
{
    std::string token;

    // 1. Active color
    if (iss >> token) { m_side_to_move = (token == "w") ? Color::WHITE : Color::BLACK; }

    // 2. Castling availability
    if (iss >> token) {
        for (const char CHR : token) {
            switch (CHR) {
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
                    // fallthrough
                default:
                    // Skip invalid characters
                    break;
            }
        }
    }

    // 3. En passant target square
    if (iss >> token) {
        try {
            m_en_passant_square = (token == "-") ? Square::NONE : stringToSquare(token);
        }
        catch (const std::invalid_argument&) {
            m_en_passant_square = Square::NONE;
        }
    }

    // 4. Halfmove clock
    if (iss >> token) {
        try {
            m_halfmove_clock = std::stoi(token);
        }
        catch (const std::invalid_argument&) {
            m_halfmove_clock = 0;
        }
    }

    // 5. Fullmove number
    if (iss >> token) {
        try {
            m_fullmove_number = std::stoi(token);
        }
        catch (const std::invalid_argument&) {
            m_fullmove_number = 1;
        }
    }
}

// Helper method to generate piece placement string
auto Position::buildFenPiecePlacement() const -> std::string
{
    std::ostringstream oss;

    // Use reverse rank (r_rank) to silence `-altera-id-dependent-backward-branch`
    for (int r_rank = 0; r_rank < Constants::Board::LENGTH; ++r_rank) {
        const int RANK = Constants::Board::MAX_RANK - r_rank;

        int empty_count = 0;
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            const Square SQR = makeSquare(file, RANK);
            const Piece PIECE = pieceAt(SQR);

            if (PIECE == Piece::NONE) {
                ++empty_count;
                continue;
            }
            if (empty_count > 0) {
                oss << empty_count;
                empty_count = 0;
            }
            oss << pieceToChar(PIECE);
        }

        if (empty_count > 0) { oss << empty_count; }

        if (RANK > 0) { oss << '/'; }
    }

    return oss.str();
}

// Helper method to generate game state string
auto Position::buildFenGameState() const -> std::string
{
    std::ostringstream oss;

    // 1. Active color
    oss << ' ' << (getSideToMove() == Color::WHITE ? 'w' : 'b');

    // 2. Castling availability
    oss << ' ';
    if (getCastlingRights() == 0) { oss << '-'; }
    else {
        if (hasCastlingRight(CastlingRight::WHITE_KINGSIDE)) { oss << 'K'; }
        if (hasCastlingRight(CastlingRight::WHITE_QUEENSIDE)) { oss << 'Q'; }
        if (hasCastlingRight(CastlingRight::BLACK_KINGSIDE)) { oss << 'k'; }
        if (hasCastlingRight(CastlingRight::BLACK_QUEENSIDE)) { oss << 'q'; }
    }

    // 3. En passant target square
    oss << ' ' << squareToString(getEnPassantSquare());

    // 4. Halfmove clock
    oss << ' ' << getHalfmoveClock();

    // 5. Fullmove number
    oss << ' ' << getFullmoveNumber();

    return oss.str();
}

auto Position::computeHash() const -> HashKey
{
    HashKey hash = 0;

    // 1. Pieces
    for (int square = 0; square < Constants::Board::SQUARE_COUNT; ++square) {
        const Piece PIECE = m_pieces.at(square);
        if (PIECE != Piece::NONE) {
            hash ^= Zobrist::getPieceSquareKey(PIECE, static_cast<Square>(square));
        }
    }

    // 2. Side to move
    if (m_side_to_move == Color::BLACK) { hash ^= Zobrist::getSideToMoveKey(); }

    // 3. Castling rights
    hash ^= Zobrist::getCastlingKey(m_castling_rights);

    // 4. En passant square
    if (m_en_passant_square != Square::NONE) {
        hash ^= Zobrist::getEnPassantKey(m_en_passant_square);
    }

    return hash;
}
} // namespace Chess