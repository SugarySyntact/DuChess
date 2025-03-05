#include <gtest/gtest.h>

#include "types.h"

using namespace Chess;

TEST(TypesTest, PieceColorConversion)
{
    // Test white pieces
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_PAWN), Color::WHITE);
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_KNIGHT), Color::WHITE);
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_BISHOP), Color::WHITE);
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_ROOK), Color::WHITE);
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_QUEEN), Color::WHITE);
    EXPECT_EQ(Util::getPieceColor(Piece::WHITE_KING), Color::WHITE);

    // Test black pieces
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_PAWN), Color::BLACK);
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_KNIGHT), Color::BLACK);
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_BISHOP), Color::BLACK);
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_ROOK), Color::BLACK);
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_QUEEN), Color::BLACK);
    EXPECT_EQ(Util::getPieceColor(Piece::BLACK_KING), Color::BLACK);

    // Test NONE
    EXPECT_EQ(Util::getPieceColor(Piece::NONE), Color::NONE);
}

TEST(TypesTest, PieceTypeConversion)
{
    // Test white pieces
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_PAWN), PieceType::PAWN);
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_KNIGHT), PieceType::KNIGHT);
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_BISHOP), PieceType::BISHOP);
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_ROOK), PieceType::ROOK);
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_QUEEN), PieceType::QUEEN);
    EXPECT_EQ(Util::getPieceType(Piece::WHITE_KING), PieceType::KING);

    // Test black pieces
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_PAWN), PieceType::PAWN);
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_KNIGHT), PieceType::KNIGHT);
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_BISHOP), PieceType::BISHOP);
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_ROOK), PieceType::ROOK);
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_QUEEN), PieceType::QUEEN);
    EXPECT_EQ(Util::getPieceType(Piece::BLACK_KING), PieceType::KING);

    // Test NONE
    EXPECT_EQ(Util::getPieceType(Piece::NONE), PieceType::NONE);
}

TEST(TypesTest, MakePiece)
{
    // Test white pieces
    EXPECT_EQ(Util::makePiece(PieceType::PAWN, Color::WHITE), Piece::WHITE_PAWN);
    EXPECT_EQ(Util::makePiece(PieceType::KNIGHT, Color::WHITE), Piece::WHITE_KNIGHT);
    EXPECT_EQ(Util::makePiece(PieceType::BISHOP, Color::WHITE), Piece::WHITE_BISHOP);
    EXPECT_EQ(Util::makePiece(PieceType::ROOK, Color::WHITE), Piece::WHITE_ROOK);
    EXPECT_EQ(Util::makePiece(PieceType::QUEEN, Color::WHITE), Piece::WHITE_QUEEN);
    EXPECT_EQ(Util::makePiece(PieceType::KING, Color::WHITE), Piece::WHITE_KING);

    // Test black pieces
    EXPECT_EQ(Util::makePiece(PieceType::PAWN, Color::BLACK), Piece::BLACK_PAWN);
    EXPECT_EQ(Util::makePiece(PieceType::KNIGHT, Color::BLACK), Piece::BLACK_KNIGHT);
    EXPECT_EQ(Util::makePiece(PieceType::BISHOP, Color::BLACK), Piece::BLACK_BISHOP);
    EXPECT_EQ(Util::makePiece(PieceType::ROOK, Color::BLACK), Piece::BLACK_ROOK);
    EXPECT_EQ(Util::makePiece(PieceType::QUEEN, Color::BLACK), Piece::BLACK_QUEEN);
    EXPECT_EQ(Util::makePiece(PieceType::KING, Color::BLACK), Piece::BLACK_KING);

    // Test edge cases
    EXPECT_EQ(Util::makePiece(PieceType::PAWN, Color::NONE), Piece::NONE);
    EXPECT_EQ(Util::makePiece(PieceType::NONE, Color::WHITE), Piece::NONE);
}

TEST(TypesTest, FileAndRank)
{
    // Test file calculations
    EXPECT_EQ(Util::getFile(Square::A1), 0);
    EXPECT_EQ(Util::getFile(Square::H1), 7);
    EXPECT_EQ(Util::getFile(Square::A8), 0);
    EXPECT_EQ(Util::getFile(Square::H8), 7);
    EXPECT_EQ(Util::getFile(Square::E4), 4);

    // Test rank calculations
    EXPECT_EQ(Util::getRank(Square::A1), 0);
    EXPECT_EQ(Util::getRank(Square::H1), 0);
    EXPECT_EQ(Util::getRank(Square::A8), 7);
    EXPECT_EQ(Util::getRank(Square::H8), 7);
    EXPECT_EQ(Util::getRank(Square::E4), 3);

    // Test make square
    EXPECT_EQ(Util::makeSquare(0, 0), Square::A1);
    EXPECT_EQ(Util::makeSquare(7, 0), Square::H1);
    EXPECT_EQ(Util::makeSquare(0, 7), Square::A8);
    EXPECT_EQ(Util::makeSquare(7, 7), Square::H8);
    EXPECT_EQ(Util::makeSquare(4, 3), Square::E4);

    // Test invalid inputs
    EXPECT_EQ(Util::makeSquare(-1, 0), Square::NONE);
    EXPECT_EQ(Util::makeSquare(0, -1), Square::NONE);
    EXPECT_EQ(Util::makeSquare(8, 0), Square::NONE);
    EXPECT_EQ(Util::makeSquare(0, 8), Square::NONE);

    // Test edge cases
    EXPECT_EQ(Util::getFile(Square::NONE), -1);
    EXPECT_EQ(Util::getRank(Square::NONE), -1);
}
