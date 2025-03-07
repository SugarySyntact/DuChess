#include <unordered_set>

#include <gtest/gtest.h>

#include "compiler_macros.h"
#include "position.h"
#include "zobrist.h"

using namespace Chess;

class PositionTest : public ::testing::Test {
protected:
    void SetUp() override { Zobrist::init(); }
};

TEST_F(PositionTest, FenConversion)
{
    const std::vector<std::string> FENS = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",             // Starting position
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", // Position 2
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",                            // Position 3
        "r1bqkbnr/pp1ppppp/2n5/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 1",     // Sicilian defense
    };

    UNROLL_PARTIAL
    for (const auto& fen : FENS) {
        Position pos(fen);
        EXPECT_EQ(fen, pos.toFen());
    }
}

TEST_F(PositionTest, HashUniqueness)
{
    std::unordered_set<HashKey> hashes;

    const std::vector<std::string> FENS = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",         // Starting position
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",      // After 1. e4
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",    // After 1...c5
        "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",   // After 2. Nf3
        "r1bqkbnr/pp1ppppp/2n5/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3", // After 2...Nc6
    };

    UNROLL_PARTIAL
    for (const auto& fen : FENS) {
        Position pos(fen);
        HashKey hash = pos.hash();

        // Each position should have a unique hash
        EXPECT_EQ(hashes.end(), hashes.find(hash));
        hashes.insert(hash);
    }
}

TEST_F(PositionTest, HashRelevantDifferences)
{
    const Position POS1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // Same position but black to move
    const Position POS2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    EXPECT_NE(POS1.hash(), POS2.hash());

    // Same position but no castling rights
    const Position POS3("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
    EXPECT_NE(POS1.hash(), POS3.hash());

    // Same position but with en passant square
    const Position POS4("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    EXPECT_NE(POS1.hash(), POS4.hash());
}

TEST_F(PositionTest, PositionEquality)
{
    const Position POS1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const Position POS2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const Position POS3("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    // Same positions should be equal
    EXPECT_EQ(POS1, POS2);

    // Different positions should not be equal
    EXPECT_NE(POS1, POS3);
}

TEST_F(PositionTest, IrrelevantFenDifferences)
{
    // Different move counters shouldn't affect equality check (for now)
    const Position POS1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const Position POS2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 10 20");

    // Positions should still be considered the same
    EXPECT_EQ(POS1.hash(), POS2.hash());
    EXPECT_EQ(POS1, POS2);
}

TEST_F(PositionTest, DefaultPosition)
{
    Position pos;

    // Check side to move
    EXPECT_EQ(Color::WHITE, pos.getSideToMove());

    // Check castling rights
    EXPECT_TRUE(pos.hasCastlingRight(CastlingRight::WHITE_KINGSIDE));
    EXPECT_TRUE(pos.hasCastlingRight(CastlingRight::WHITE_QUEENSIDE));
    EXPECT_TRUE(pos.hasCastlingRight(CastlingRight::BLACK_KINGSIDE));
    EXPECT_TRUE(pos.hasCastlingRight(CastlingRight::BLACK_QUEENSIDE));

    // Check en passant square
    EXPECT_EQ(Square::NONE, pos.getEnPassantSquare());

    // Check move counters
    EXPECT_EQ(0, pos.getHalfmoveClock());
    EXPECT_EQ(1, pos.getFullmoveNumber());

    // Check pieces
    EXPECT_EQ(Piece::WHITE_ROOK, pos.pieceAt(Square::A1));
    EXPECT_EQ(Piece::WHITE_KNIGHT, pos.pieceAt(Square::B1));
    EXPECT_EQ(Piece::WHITE_BISHOP, pos.pieceAt(Square::C1));
    EXPECT_EQ(Piece::WHITE_QUEEN, pos.pieceAt(Square::D1));
    EXPECT_EQ(Piece::WHITE_KING, pos.pieceAt(Square::E1));
    EXPECT_EQ(Piece::BLACK_ROOK, pos.pieceAt(Square::A8));
    EXPECT_EQ(Piece::BLACK_KING, pos.pieceAt(Square::E8));

    // Check hash is non-zero
    EXPECT_NE(0ULL, pos.hash());
};