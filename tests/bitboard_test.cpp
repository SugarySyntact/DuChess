#include <gtest/gtest.h>

#include "bitboard.h"
#include "constants.h"

using namespace Chess;
using namespace Util;

class BitboardTest : public ::testing::Test {
protected:
    void SetUp() override { Bitboards::init(); }
};

TEST_F(BitboardTest, BitOperations)
{
    // Test bit setting and clearing
    Bitboard bitb = 0;

    // Set bits
    setBit(bitb, Square::E4);
    EXPECT_TRUE(testBit(bitb, Square::E4));
    EXPECT_FALSE(testBit(bitb, Square::E5));

    setBit(bitb, Square::A1);
    EXPECT_TRUE(testBit(bitb, Square::A1));
    EXPECT_TRUE(testBit(bitb, Square::E4));

    // Clear bits
    clearBit(bitb, Square::E4);
    EXPECT_FALSE(testBit(bitb, Square::E4));
    EXPECT_TRUE(testBit(bitb, Square::A1));

    // Square to bitboard conversion
    const Bitboard SINGLE = squareBB(Square::H8);
    EXPECT_TRUE(testBit(SINGLE, Square::H8));
    EXPECT_EQ(Bitboards::popCount(SINGLE), 1);
}

TEST_F(BitboardTest, LsbMsb)
{
    constexpr unsigned int TEST_H8 = 63;

    // Test with empty bitboard
    EXPECT_EQ(Bitboards::lsb(0), Square::NONE);
    EXPECT_EQ(Bitboards::msb(0), Square::NONE);

    // Test with single bits
    EXPECT_EQ(Bitboards::lsb(1ULL), Square::A1);
    EXPECT_EQ(Bitboards::msb(1ULL), Square::A1);

    EXPECT_EQ(Bitboards::lsb(1ULL << TEST_H8), Square::H8);
    EXPECT_EQ(Bitboards::msb(1ULL << TEST_H8), Square::H8);

    // Test with multiple bits
    const Bitboard MULTIPLE = (1ULL << static_cast<unsigned>(Square::A1)) |
                              (1ULL << static_cast<unsigned>(Square::E4)) |
                              (1ULL << static_cast<unsigned>(Square::H8));

    EXPECT_EQ(Bitboards::lsb(MULTIPLE), Square::A1);
    EXPECT_EQ(Bitboards::msb(MULTIPLE), Square::H8);
}

TEST_F(BitboardTest, PopCount)
{
    constexpr unsigned int TEST_H8 = 63;

    // Empty bitboard
    EXPECT_EQ(Bitboards::popCount(0), 0);

    // Single bit
    EXPECT_EQ(Bitboards::popCount(1ULL), 1);
    EXPECT_EQ(Bitboards::popCount(1ULL << TEST_H8), 1);

    // Multiple bits
    Bitboard multiple = (1ULL << static_cast<unsigned>(Square::A1)) |
                        (1ULL << static_cast<unsigned>(Square::E4)) |
                        (1ULL << static_cast<unsigned>(Square::H8));
    EXPECT_EQ(Bitboards::popCount(multiple), 3);

    // Full bitboard
    EXPECT_EQ(Bitboards::popCount(~0ULL), 64);
}

TEST_F(BitboardTest, PopLsb)
{
    // Test with single bit
    Bitboard single = 1ULL;
    EXPECT_EQ(Bitboards::popLsb(single), 1ULL);
    EXPECT_EQ(single, 0ULL);

    // Test with multiple bits
    Bitboard multiple = (1ULL << static_cast<unsigned>(Square::A1)) |
                        (1ULL << static_cast<unsigned>(Square::E4)) |
                        (1ULL << static_cast<unsigned>(Square::H8));

    EXPECT_EQ(Bitboards::popLsb(multiple), 1ULL);
    EXPECT_EQ(Bitboards::popCount(multiple), 2);
    EXPECT_EQ(Bitboards::popLsb(multiple), 1ULL << static_cast<unsigned>(Square::E4));
    EXPECT_EQ(Bitboards::popCount(multiple), 1);
    EXPECT_EQ(Bitboards::popLsb(multiple), 1ULL << static_cast<unsigned>(Square::H8));
    EXPECT_EQ(multiple, 0ULL);
}

TEST_F(BitboardTest, PredefinedBitboards)
{
    // Test file masks
    for (int file = 0; file < Constants::Board::LENGTH; ++file) {
        EXPECT_EQ(Bitboards::popCount(Bitboards::files.at(file)), 8);
        for (int rank = 0; rank < Constants::Board::LENGTH; ++rank) {
            EXPECT_TRUE(testBit(Bitboards::files.at(file), makeSquare(file, rank)));
        }
    }

    // Test rank masks
    for (int rank = 0; rank < Constants::Board::LENGTH; ++rank) {
        EXPECT_EQ(Bitboards::popCount(Bitboards::ranks.at(rank)), 8);
        for (int file = 0; file < Constants::Board::LENGTH; ++file) {
            EXPECT_TRUE(testBit(Bitboards::ranks.at(rank), makeSquare(file, rank)));
        }
    }

    // Test diagonal masks (a few examples)
    // Main diagonal a1-h8
    EXPECT_EQ(Bitboards::popCount(Bitboards::diagonals.at(7)), 8);
    EXPECT_TRUE(testBit(Bitboards::diagonals.at(7), Square::A1));
    EXPECT_TRUE(testBit(Bitboards::diagonals.at(7), Square::H8));

    // Anti-diagonal a8-h1
    EXPECT_EQ(Bitboards::popCount(Bitboards::anti_diagonals.at(7)), 8);
    EXPECT_TRUE(testBit(Bitboards::anti_diagonals.at(7), Square::A8));
    EXPECT_TRUE(testBit(Bitboards::anti_diagonals.at(7), Square::H1));
}

TEST_F(BitboardTest, ShiftOperations)
{
    // Test northOne (up one rank)
    const Bitboard BITB = squareBB(Square::E4);
    EXPECT_EQ(northOne(BITB), squareBB(Square::E5));
    EXPECT_EQ(northOne(squareBB(Square::E8)), 0); // Edge of board

    // Test southOne (down one rank)
    EXPECT_EQ(southOne(BITB), squareBB(Square::E3));
    EXPECT_EQ(southOne(squareBB(Square::E1)), 0); // Edge of board

    // Test eastOne (right one file)
    EXPECT_EQ(eastOne(BITB), squareBB(Square::F4));
    EXPECT_EQ(eastOne(squareBB(Square::H4)), 0); // Edge of board

    // Test westOne (left one file)
    EXPECT_EQ(westOne(BITB), squareBB(Square::D4));
    EXPECT_EQ(westOne(squareBB(Square::A4)), 0); // Edge of board

    // Test combined shifts
    EXPECT_EQ(northEastOne(BITB), squareBB(Square::F5));
    EXPECT_EQ(northWestOne(BITB), squareBB(Square::D5));
    EXPECT_EQ(southEastOne(BITB), squareBB(Square::F3));
    EXPECT_EQ(southWestOne(BITB), squareBB(Square::D3));
}