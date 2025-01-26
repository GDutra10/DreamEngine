//#include <gtest/gtest.h>
//
//#include "../../../src/Core/Math/Vector3.h"
//
//namespace DreamEngine::CoreTests::Math
//{
//	using namespace Core::Math;
//
//	TEST(Vector3Test, ShouldSumCorrectly)
//	{
//		// arrange
//		Vector3 vec3;
//		Vector3 another = { 10, 5, 4 };
//
//		// act
//		vec3.Sum(another);
//
//		// asserts
//		EXPECT_EQ(vec3.x, 10);
//		EXPECT_EQ(vec3.y, 5);
//		EXPECT_EQ(vec3.z, 4);
//	}
//
//	TEST(Vector3Test, ShouldSubtractCorrectly)
//	{
//		// arrange
//		Vector3 vec3;
//		Vector3 another = { 10, 5, 4 };
//
//		// act
//		vec3.Subtract(another);
//
//		// asserts
//		EXPECT_EQ(vec3.x, -10);
//		EXPECT_EQ(vec3.y, -5);
//		EXPECT_EQ(vec3.z, -4);
//	}
//
//	TEST(Vector3Test, ShouldMultiplyCorrectly)
//	{
//		// arrange
//		Vector3 vec3 = { 1, 2, 3 };
//		Vector3 another = { 10, 5, 4 };
//
//		// act
//		vec3.Multiply(another);
//
//		// asserts
//		EXPECT_EQ(vec3.x, 10);
//		EXPECT_EQ(vec3.y, 10);
//		EXPECT_EQ(vec3.z, 12);
//	}
//
//	TEST(Vector3Test, ShouldDivideCorrectly)
//	{
//		// arrange
//		Vector3 vec3 = { 10, 20, 5 };
//		Vector3 another = { 5, 4, 5 };
//
//		// act
//		vec3.Divide(another);
//
//		// asserts
//		EXPECT_EQ(vec3.x, 2);
//		EXPECT_EQ(vec3.y, 5);
//		EXPECT_EQ(vec3.z, 1);
//	}
//
//
//	TEST(Vector3Test, ShouldMultiplyScalarCorrectly)
//	{
//		// arrange
//		Vector3 vec3 = { 1, 2, 3};
//
//		// act
//		vec3.Multiply(2);
//
//		// asserts
//		EXPECT_EQ(vec3.x, 2);
//		EXPECT_EQ(vec3.y, 4);
//		EXPECT_EQ(vec3.z, 6);
//	}
//
//	TEST(Vector3Test, ShouldDivideScalarCorrectly)
//	{
//		// arrange
//		Vector3 vec3 = { 2, 4, 6 };
//
//		// act
//		vec3.Divide(2);
//
//		// asserts
//		EXPECT_EQ(vec3.x, 1);
//		EXPECT_EQ(vec3.y, 2);
//		EXPECT_EQ(vec3.z, 3);
//	}
//
//	TEST(Vector3Test, ShouldNegateCorrectly)
//	{
//		// arrange
//		Vector3 vec3 = { 2, 4, 6 };
//
//		// act
//		vec3.Negate();
//
//		// asserts
//		EXPECT_EQ(vec3.x, -2);
//		EXPECT_EQ(vec3.y, -4);
//		EXPECT_EQ(vec3.z, -6);
//	}
//}