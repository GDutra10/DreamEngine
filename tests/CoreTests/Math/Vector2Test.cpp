//#include <gtest/gtest.h>
//
//#include "../../../src/Core/Math/Vector2.h"
//
//namespace DreamEngine::CoreTests::Math
//{
//	using namespace Core::Math;
//
//	TEST(Vector2Test, ShouldSumCorrectly)
//	{
//		// arrange
//		Vector2 vec2;
//		Vector2 another = { 10, 5 };
//
//		// act
//		vec2.Sum(another);
//
//		// asserts
//		EXPECT_EQ(vec2.x, 10);
//		EXPECT_EQ(vec2.y, 5);
//	}
//
//	TEST(Vector2Test, ShouldSubtractCorrectly)
//	{
//		// arrange
//		Vector2 vec2;
//		Vector2 another = { 10, 5 };
//
//		// act
//		vec2.Subtract(another);
//
//		// asserts
//		EXPECT_EQ(vec2.x, -10);
//		EXPECT_EQ(vec2.y, -5);
//	}
//
//	TEST(Vector2Test, ShouldMultiplyCorrectly)
//	{
//		// arrange
//		Vector2 vec2 = { 1, 2 };
//		Vector2 another = { 10, 5 };
//
//		// act
//		vec2.Multiply(another);
//
//		// asserts
//		EXPECT_EQ(vec2.x, 10);
//		EXPECT_EQ(vec2.y, 10);
//	}
//
//	TEST(Vector2Test, ShouldDivideCorrectly)
//	{
//		// arrange
//		Vector2 vec2 = { 10, 20 };
//		Vector2 another = { 5, 4 };
//
//		// act
//		vec2.Divide(another);
//
//		// asserts
//		EXPECT_EQ(vec2.x, 2);
//		EXPECT_EQ(vec2.y, 5);
//	}
//
//
//	TEST(Vector2Test, ShouldMultiplyScalarCorrectly)
//	{
//		// arrange
//		Vector2 vec2 = { 1, 2 };
//
//		// act
//		vec2.Multiply(2);
//
//		// asserts
//		EXPECT_EQ(vec2.x, 2);
//		EXPECT_EQ(vec2.y, 4);
//	}
//
//	TEST(Vector2Test, ShouldDivideScalarCorrectly)
//	{
//		// arrange
//		Vector2 vec2 = { 2, 4 };
//
//		// act
//		vec2.Divide(2);
//
//		// asserts
//		EXPECT_EQ(vec2.x, 1);
//		EXPECT_EQ(vec2.y, 2);
//	}
//
//	TEST(Vector2Test, ShouldNegateCorrectly)
//	{
//		// arrange
//		Vector2 vec2 = { 2, 4 };
//
//		// act
//		vec2.Negate();
//
//		// asserts
//		EXPECT_EQ(vec2.x, -2);
//		EXPECT_EQ(vec2.y, -4);
//	}
//}