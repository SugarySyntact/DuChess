#ifndef CHESS_ZOBRIST_H
#define CHESS_ZOBRIST_H

#include <array>

#include "constants.h"
#include "types.h"

namespace Chess {

class Zobrist {
public:
    static auto init() -> void;
    static auto getPieceSquareKey(Piece piece, Square square) -> HashKey;
    static auto getSideToMoveKey() -> HashKey;
    static auto getCastlingKey(CastlingRightsBitField rights) -> HashKey;
    static auto getEnPassantKey(Square square) -> HashKey;

private:
    static std::array<std::array<HashKey, Constants::Board::SQUARE_COUNT>,
                      Constants::Zobrist::PIECE_COUNT>
        piece_square_keys;
    static HashKey side_to_move_key;
    static std::array<HashKey, Constants::Zobrist::CASTLING_COMBINATIONS> castling_keys;
    static std::array<HashKey, Constants::Board::SQUARE_COUNT_WITH_EMPTY> en_passant_keys;
};

} // namespace Chess

#endif // CHESS_ZOBRIST_H