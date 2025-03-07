#include "zobrist.h"

#include <cstdint>
#include <random>

#include "compiler_macros.h"
#include "constants.h"

namespace Chess {

using namespace Util;

std::array<std::array<HashKey, Constants::Board::SQUARE_COUNT>, Constants::Zobrist::PIECE_COUNT>
    Zobrist::piece_square_keys;
HashKey Zobrist::side_to_move_key;
std::array<HashKey, Constants::Zobrist::CASTLING_COMBINATIONS> Zobrist::castling_keys;
std::array<HashKey, Constants::Board::SQUARE_COUNT_WITH_EMPTY> Zobrist::en_passant_keys;

auto Zobrist::init() -> void
{
    constexpr uint64_t FIXED_SEED = 0x71E69E733F44B6F4ULL;

    // NOLINTNEXTLINE(cert-msc51-cpp,cert-msc32-c) - For reproducible testing env
    std::mt19937_64 rng(FIXED_SEED);

    for (auto& piece : piece_square_keys) {
        UNROLL_PARTIAL
        for (auto& square : piece) { square = rng(); }
    }

    side_to_move_key = rng();

    UNROLL_PARTIAL
    for (auto& keys : castling_keys) { keys = rng(); }
    UNROLL_PARTIAL
    for (auto& keys : en_passant_keys) { keys = rng(); }
}

auto Zobrist::getPieceSquareKey(Piece piece, Square square) -> HashKey
{
    if (piece == Piece::NONE) { return 0; }
    if (square == Square::NONE) { return 0; }

    return piece_square_keys.at(toIdx(piece)).at(toIdx(square));
}

auto Zobrist::getSideToMoveKey() -> HashKey { return side_to_move_key; }

auto Zobrist::getCastlingKey(CastlingRightsBitField rights) -> HashKey
{
    constexpr CastlingRightsBitField LOW_NIBBLE = 0xF;
    return castling_keys.at(rights & LOW_NIBBLE);
}

auto Zobrist::getEnPassantKey(Square square) -> HashKey
{
    if (square == Square::NONE) { return en_passant_keys.at(Constants::Board::SQUARE_COUNT); }
    return en_passant_keys.at(toIdx(square));
}

} // namespace Chess