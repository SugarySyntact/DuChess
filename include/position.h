#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include <array>
#include <string>

#include "constants.h"
#include "types.h"

namespace Chess {

class Position {
public:
    Position();
    explicit Position(const std::string& fen);

    [[nodiscard]] auto pieceAt(Square square) const -> Piece;
    [[nodiscard]] auto getPieceBitboard(PieceType type, Color color) const -> Bitboard;
    [[nodiscard]] auto getColorBitboard(Color color) const -> Bitboard;
    [[nodiscard]] auto getOccupiedBitboard() const -> Bitboard;

    [[nodiscard]] auto getSideToMove() const -> Color;
    [[nodiscard]] auto hasCastlingRight(CastlingRight right) const -> bool;
    [[nodiscard]] auto getCastlingRights() const -> CastlingRightsBitField;
    [[nodiscard]] auto getEnPassantSquare() const -> Square;
    [[nodiscard]] auto getHalfmoveClock() const -> int;
    [[nodiscard]] auto getFullmoveNumber() const -> int;

    [[nodiscard]] auto hash() const -> HashKey;

    [[nodiscard]] auto toFen() const -> std::string;

    auto print() const -> void;

    auto operator==(const Position& other) const -> bool;
    auto operator!=(const Position& other) const -> bool;

private:
    std::array<std::array<Bitboard, Constants::Board::PIECE_TYPE_COUNT>,
               Constants::Board::COLOR_COUNT>
        m_piece_bitboards;
    std::array<Bitboard, Constants::Board::COLOR_COUNT> m_color_bitboards;
    std::array<Piece, Constants::Board::SQUARE_COUNT> m_pieces;

    Color m_side_to_move;
    CastlingRightsBitField m_castling_rights;
    Square m_en_passant_square;
    int m_halfmove_clock;
    int m_fullmove_number;

    HashKey m_position_hash;

    [[nodiscard]] auto computeHash() const -> HashKey;
};

} // namespace Chess

#endif // CHESS_POSITION_H