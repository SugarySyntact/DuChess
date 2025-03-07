#include <unordered_set>

#include <gtest/gtest.h>

#include "compiler_macros.h"
#include "constants.h"
#include "types.h"
#include "zobrist.h"

using namespace Chess;
using namespace Util;

class ZobristTest : public ::testing::Test {
protected:
    void SetUp() override { Zobrist::init(); }
};

TEST_F(ZobristTest, PieceSquareKeysUnique)
{
    std::unordered_set<HashKey> keys;

    // Check that all piece-square keys are unique
    for (int piece = 1; piece < Constants::Zobrist::PIECE_COUNT; ++piece) { // Skip NONE (0)
        UNROLL_LOOP
        for (int square = 0; square < Constants::Board::SQUARE_COUNT; ++square) {
            HashKey key =
                Zobrist::getPieceSquareKey(fromIdx<Piece>(piece), fromIdx<Square>(square));

            // The key should not be zero
            EXPECT_NE(0ULL, key);

            // The key should be unique
            EXPECT_EQ(keys.end(), keys.find(key));
            keys.insert(key);
        }
    }
}

TEST_F(ZobristTest, PieceSquareKeysConsistent)
{
    const Piece PIECE = Piece::WHITE_QUEEN;
    const Square SQUARE = Square::E4;

    const HashKey KEY1 = Zobrist::getPieceSquareKey(PIECE, SQUARE);
    const HashKey KEY2 = Zobrist::getPieceSquareKey(PIECE, SQUARE);

    EXPECT_EQ(KEY1, KEY2);
}

TEST_F(ZobristTest, CastlingKeysUnique)
{
    std::unordered_set<HashKey> keys;

    // Check that all castling keys are unique
    UNROLL_LOOP
    for (int rights = 0; rights < Constants::Zobrist::CASTLING_COMBINATIONS; ++rights) {
        const HashKey KEY = Zobrist::getCastlingKey(rights);

        // The key should not be zero
        EXPECT_NE(0ULL, KEY);

        // The key should be unique
        EXPECT_EQ(keys.end(), keys.find(KEY));
        keys.insert(KEY);
    }
}

TEST_F(ZobristTest, EnPassantKeysUnique)
{
    std::unordered_set<HashKey> keys;

    // Check that all en passant keys are unique
    UNROLL_LOOP
    for (int square = 0; square <= Constants::Board::SQUARE_COUNT; ++square) {
        const Square SQR =
            (square < Constants::Board::SQUARE_COUNT) ? fromIdx<Square>(square) : Square::NONE;
        const HashKey KEY = Zobrist::getEnPassantKey(SQR);

        // The key should be unique
        EXPECT_EQ(keys.end(), keys.find(KEY));
        keys.insert(KEY);
    }
}

TEST_F(ZobristTest, HashConsistency)
{
    // Compute a hash using XOR operations as would be done in a Position class
    HashKey hash1 = 0;

    // Add some pieces
    hash1 ^= Zobrist::getPieceSquareKey(Piece::WHITE_KING, Square::E1);
    hash1 ^= Zobrist::getPieceSquareKey(Piece::WHITE_QUEEN, Square::D1);
    hash1 ^= Zobrist::getPieceSquareKey(Piece::BLACK_KING, Square::E8);

    // Set side to move to black
    hash1 ^= Zobrist::getSideToMoveKey();

    // Set castling rights
    hash1 ^= Zobrist::getCastlingKey(toIdx(CastlingRight::WHITE_KINGSIDE) |
                                     toIdx(CastlingRight::BLACK_KINGSIDE));

    // Compute the same hash again
    HashKey hash2 = 0;
    hash2 ^= Zobrist::getPieceSquareKey(Piece::WHITE_KING, Square::E1);
    hash2 ^= Zobrist::getPieceSquareKey(Piece::WHITE_QUEEN, Square::D1);
    hash2 ^= Zobrist::getPieceSquareKey(Piece::BLACK_KING, Square::E8);
    hash2 ^= Zobrist::getSideToMoveKey();
    hash2 ^= Zobrist::getCastlingKey(toIdx(CastlingRight::WHITE_KINGSIDE) |
                                     toIdx(CastlingRight::BLACK_KINGSIDE));

    // The hashes should be identical
    EXPECT_EQ(hash1, hash2);

    // Now update the hash by making a move
    // Remove the queen from d1
    hash1 ^= Zobrist::getPieceSquareKey(Piece::WHITE_QUEEN, Square::D1);
    // Add the queen to d4
    hash1 ^= Zobrist::getPieceSquareKey(Piece::WHITE_QUEEN, Square::D4);
    // Change side to move
    hash1 ^= Zobrist::getSideToMoveKey();

    // Verify the hash changes when the position changes
    EXPECT_NE(hash1, hash2);
}

TEST_F(ZobristTest, IncrementalUpdates)
{
    // Compute full hash
    HashKey full_hash = 0;
    full_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_KING, Square::E1);
    full_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_PAWN, Square::E2);
    full_hash ^= Zobrist::getPieceSquareKey(Piece::BLACK_KING, Square::E8);
    full_hash ^= Zobrist::getCastlingKey(toIdx(CastlingRight::ALL));

    // Now copy and incrementally update
    HashKey incremental_hash = full_hash;

    // Move the pawn from e2 to e4
    incremental_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_PAWN, Square::E2); // Remove from e2
    incremental_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_PAWN, Square::E4); // Add to e4
    incremental_hash ^= Zobrist::getSideToMoveKey();          // Change side to move
    incremental_hash ^= Zobrist::getEnPassantKey(Square::E3); // Set en passant square

    // Compute the new hash from scratch
    HashKey new_full_hash = 0;
    new_full_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_KING, Square::E1);
    new_full_hash ^= Zobrist::getPieceSquareKey(Piece::WHITE_PAWN, Square::E4); // Pawn on e4 now
    new_full_hash ^= Zobrist::getPieceSquareKey(Piece::BLACK_KING, Square::E8);
    new_full_hash ^= Zobrist::getCastlingKey(toIdx(CastlingRight::ALL));
    new_full_hash ^= Zobrist::getSideToMoveKey();          // Black to move
    new_full_hash ^= Zobrist::getEnPassantKey(Square::E3); // En passant on e3

    // The incremental hash should match the full recomputation
    EXPECT_EQ(new_full_hash, incremental_hash);
}