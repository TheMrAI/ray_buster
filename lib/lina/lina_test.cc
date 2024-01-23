#include <gtest/gtest.h>

#include "lib/lina/lina.h"
#include <array>
#include <span>

auto checkEquality(std::span<double, 3> result, std::span<double, 3> expected)
{
  for (std::size_t i = 0; i < result.size(); i++) { EXPECT_DOUBLE_EQ(result[i], expected[i]); }
}

TEST(Add, UnitToZero)
{
  auto lhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 0.0 };
  auto expected = std::array<double, 3>{ 1.0, 1.0, 1.0 };

  auto result = lina::add(lhs, rhs);
  checkEquality(result, expected);
  result = lina::add(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Add, UnitToInverse)
{
  auto lhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto rhs = std::array<double, 3>{ -1.0, -1.0, -1.0 };
  auto expected = std::array<double, 3>{ 0.0, 0.0, 0.0 };

  auto result = lina::add(lhs, rhs);
  checkEquality(result, expected);
  result = lina::add(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Add, Random)
{
  auto lhs = std::array<double, 3>{ 3.565555, 2.123, 1.46899 };
  auto rhs = std::array<double, 3>{ -2.12, 7.888675, 12.3 };
  auto expected = std::array<double, 3>{ 1.445555, 10.011675, 13.76899 };

  auto result = lina::add(lhs, rhs);
  checkEquality(result, expected);
  result = lina::add(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Sub, UnitFromZero)
{
  auto lhs = std::array<double, 3>{ 0.0, 0.0, 0.0 };
  auto rhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto expected = std::array<double, 3>{ -1.0, -1.0, -1.0 };
  auto result = lina::sub(lhs, rhs);

  checkEquality(result, expected);
}

TEST(Sub, ZeroFromUnit)
{
  auto lhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 0.0 };
  auto expected = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto result = lina::sub(lhs, rhs);

  checkEquality(result, expected);
}

TEST(Sub, UnitFromUnit)
{
  auto lhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto rhs = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto expected = std::array<double, 3>{ 0.0, 0.0, 0.0 };
  auto result = lina::sub(lhs, rhs);

  checkEquality(result, expected);
}

TEST(Sub, Random)
{
  auto lhs = std::array<double, 3>{ 3.565555, 2.123, 1.46899 };
  auto rhs = std::array<double, 3>{ -2.12, 7.888675, 12.3 };
  auto expected = std::array<double, 3>{ 5.685555, -5.765675, -10.83101 };
  auto result = lina::sub(lhs, rhs);

  checkEquality(result, expected);
}

TEST(Scale, UnitByZero)
{
  auto vector = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto expected = std::array<double, 3>{ 0.0, 0.0, 0.0 };
  auto result = lina::scale(0.0, vector);

  checkEquality(result, expected);
  result = lina::scale(vector, 0.0);
  checkEquality(result, expected);
}

TEST(Scale, UnitByOne)
{
  auto vector = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto expected = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto result = lina::scale(1.0, vector);

  checkEquality(result, expected);
  result = lina::scale(vector, 1.0);
  checkEquality(result, expected);
}

TEST(Scale, UnitByMinusOne)
{
  auto vector = std::array<double, 3>{ 1.0, 1.0, 1.0 };
  auto expected = std::array<double, 3>{ -1.0, -1.0, -1.0 };
  auto result = lina::scale(-1.0, vector);

  checkEquality(result, expected);
  result = lina::scale(vector, -1.0);
  checkEquality(result, expected);
}

TEST(Scale, RandomByRandom)
{
  auto vector = std::array<double, 3>{ -2.12, 7.888675, 12.3 };
  auto expected = std::array<double, 3>{ 6.6638861, -24.7968073961875, -38.66311275 };
  auto result = lina::scale(-3.1433425, vector);

  checkEquality(result, expected);
  result = lina::scale(vector, -3.1433425);
  checkEquality(result, expected);
}

TEST(Cross, IhatJHat)
{
  auto lhs = std::array<double, 3>{ 1.0, 0.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 1.0, 0.0 };
  auto expected = std::array<double, 3>{ 0.0, 0.0, 1.0 };
  auto result = lina::cross(lhs, rhs);

  checkEquality(result, expected);
  // reversed
  expected = std::array<double, 3>{ 0.0, 0.0, -1.0 };
  result = lina::cross(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Cross, JhatKHat)
{
  auto lhs = std::array<double, 3>{ 0.0, 1.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 1.0 };
  auto expected = std::array<double, 3>{ 1.0, 0.0, 0.0 };
  auto result = lina::cross(lhs, rhs);

  checkEquality(result, expected);
  // reversed
  expected = std::array<double, 3>{ -1.0, 0.0, 0.0 };
  result = lina::cross(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Cross, IhatKHat)
{
  auto lhs = std::array<double, 3>{ 1.0, 0.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 1.0 };
  auto expected = std::array<double, 3>{ 0.0, -1.0, 0.0 };
  auto result = lina::cross(lhs, rhs);

  checkEquality(result, expected);
  // reversed
  expected = std::array<double, 3>{ 0.0, 1.0, 0.0 };
  result = lina::cross(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Cross, Random)
{
  auto lhs = std::array<double, 3>{ 3.565555, 2.123, 1.46899 };
  auto rhs = std::array<double, 3>{ -2.12, 7.888675, -4.50076 };
  auto expected = std::array<double, 3>{ -21.14349816825, 12.9334485218, 32.628264589625 };
  auto result = lina::cross(lhs, rhs);

  checkEquality(result, expected);
  // reversed
  expected = std::array<double, 3>{ 21.14349816825, -12.9334485218, -32.628264589625 };
  result = lina::cross(rhs, lhs);
  checkEquality(result, expected);
}

TEST(Dot, IHatJHat)
{
  auto lhs = std::array<double, 3>{ 1.0, 0.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 1.0, 0.0 };
  auto expected = 0.0;
  auto result = lina::dot(lhs, rhs);

  EXPECT_DOUBLE_EQ(result, expected);
  result = lina::dot(rhs, lhs);
  EXPECT_DOUBLE_EQ(result, expected);
}

TEST(Dot, JHatKHat)
{
  auto lhs = std::array<double, 3>{ 0.0, 1.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 1.0 };
  auto expected = 0.0;
  auto result = lina::dot(rhs, lhs);

  EXPECT_DOUBLE_EQ(result, expected);
  result = lina::dot(rhs, lhs);
  EXPECT_DOUBLE_EQ(result, expected);
}

TEST(Dot, IHatKHat)
{
  auto lhs = std::array<double, 3>{ 1.0, 0.0, 0.0 };
  auto rhs = std::array<double, 3>{ 0.0, 0.0, 1.0 };
  auto expected = 0.0;
  auto result = lina::dot(rhs, lhs);

  EXPECT_DOUBLE_EQ(result, expected);
  result = lina::dot(rhs, lhs);
  EXPECT_DOUBLE_EQ(result, expected);
}

TEST(Dot, Random)
{
  auto lhs = std::array<double, 3>{ 3.565555, 2.123, 1.46899 };
  auto rhs = std::array<double, 3>{ -2.12, 7.888675, 12.3 };
  auto expected = 27.257257425;
  auto result = lina::dot(rhs, lhs);

  EXPECT_DOUBLE_EQ(result, expected);
  result = lina::dot(rhs, lhs);
  EXPECT_DOUBLE_EQ(result, expected);
}