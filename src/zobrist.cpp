#include "zobrist.h"

#include <random>

namespace Chess {

std::array<std::array<HashKey, Constants::Board::SQUARE_COUNT>, Constants::Zobrist::PIECE_COUNT>
    Zobrist::piece_square_keys;
HashKey Zobrist::side_to_move_key;
std::array<HashKey, Constants::Zobrist::CASTLING_COMBINATIONS> Zobrist::castling_keys;
std::array<HashKey, Constants::Board::SQUARE_COUNT + 1> Zobrist::en_passant_keys;

auto Zobrist::init() -> void
{
    constexpr uint64_t FIXED_SEED = 0x71E69E733F44B6F4ULL;

    // NOLINTNEXTLINE(cert-msc51-cpp,cert-msc32-c)
    std::mt19937_64 rng(FIXED_SEED);

    for (auto& piece : piece_square_keys) {
        for (auto& square : piece) { square = rng(); }
    }

    side_to_move_key = rng();

    for (auto& keys : castling_keys) { keys = rng(); }
    for (auto& keys : en_passant_keys) { keys = rng(); }
}

auto Zobrist::getPieceSquareKey(Piece piece, Square square) -> HashKey
{
    if (piece == Piece::NONE) { return 0; }
    if (square == Square::NONE) { return 0; }

    return piece_square_keys.at(static_cast<int>(piece)).at(static_cast<int>(square));
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
    return en_passant_keys.at(static_cast<int>(square));
}

} // namespace Chess