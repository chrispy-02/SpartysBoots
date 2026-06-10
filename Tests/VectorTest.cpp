/**
 * @file VectorTest.cpp
 * @author Elijah Porter
 */

#include <pch.h>
#include <Vector.h>

#include "gtest/gtest.h"


TEST(VectorTest, VectorConstruction)
{
    Vector v1;

    ASSERT_EQ(v1.GetX(), 0) << "Default Vector constructor; Check X";
    ASSERT_EQ(v1.GetY(), 0) << "Default Vector constructor; Check Y";

    Vector v2(12, -6);

    ASSERT_EQ(v2.GetX(), 12) << "Vector constructor; Check X";
    ASSERT_EQ(v2.GetY(), -6) << "Vector constructor; Check Y";
}

TEST(VectorTest, VectorMagnitude)
{
    Vector v1(0, 0);

    ASSERT_NEAR(v1.GetMagnitude(), 0, 0.0001) << "Zero Vector magnitude";

    Vector v2(12, -6);

    ASSERT_NEAR(v2.GetMagnitude(), 13.4164, 0.0001);
}

TEST(VectorTest, VectorNormalize)
{
    Vector v1(0, 0);

    v1.Normalize();

    ASSERT_EQ(v1.GetX(), 0) << "Normalize zero Vector; Check X";
    ASSERT_EQ(v1.GetY(), 0) << "Normalize zero Vector; Check Y";

    Vector v2(5, 5);

    ASSERT_NEAR(v2.Normalize().GetX(), 0.7071, 0.0001) << "Normalize Vector; Check X";
    ASSERT_NEAR(v2.Normalize().GetY(), 0.7071, 0.0001) << "Normalize Vector; Check Y";
}

TEST(VectorTest, VectorOperators)
{
    Vector v1(0, 0);

    auto v2 = v1 + Vector(2, 3);

    ASSERT_EQ(v2.GetX(), 2) << "Add Operator; Check X";
    ASSERT_EQ(v2.GetY(), 3) << "Add Operator; Check Y";

    v2 = v1 - Vector(2, 3);

    ASSERT_EQ(v2.GetX(), -2) << "Subtract Operator; Check X";
    ASSERT_EQ(v2.GetY(), -3) << "Subtract Operator; Check Y";

    v2 = -v2;

    ASSERT_EQ(v2.GetX(), 2) << "Negate Operator; Check X";
    ASSERT_EQ(v2.GetY(), 3) << "Negate Operator; Check Y";

    v2 = v2 * 3;

    ASSERT_EQ(v2.GetX(), 6) << "Multiply Operator; Check X";
    ASSERT_EQ(v2.GetY(), 9) << "Multiply Operator; Check Y";

    v2 = v2 / 3;

    ASSERT_EQ(v2.GetX(), 2) << "Divide Operator; Check X";
    ASSERT_EQ(v2.GetY(), 3) << "Divide Operator; Check Y";

    v1 += Vector(3, 4);

    ASSERT_EQ(v1.GetX(), 3) << "Add Equal Operator; Check X";
    ASSERT_EQ(v1.GetY(), 4) << "Add Equal Operator; Check Y";

    v1 -= Vector(1, 1);

    ASSERT_EQ(v1.GetX(), 2) << "Subtract Equal Operator; Check X";
    ASSERT_EQ(v1.GetY(), 3) << "Subtract Equal Operator; Check Y";

    v1 *= 2;

    ASSERT_EQ(v1.GetX(), 4) << "Multiply Equal Operator; Check X";
    ASSERT_EQ(v1.GetY(), 6) << "Multiply Equal Operator; Check Y";

    v1 /= 2;

    ASSERT_EQ(v1.GetX(), 2) << "Divide Equal Operator; Check X";
    ASSERT_EQ(v1.GetY(), 3) << "Divide Equal Operator; Check Y";

    (v1 *= 2) *= 3;

    ASSERT_EQ(v1.GetX(), 12) << "Operator Chaining; Check X";
    ASSERT_EQ(v1.GetY(), 18) << "Operator Chaining; Check Y";

    v1 /= 6;

    ASSERT_TRUE(v1 == Vector(2, 3));
    ASSERT_FALSE(v1 == Vector(3, 3));
    ASSERT_FALSE(v1 != Vector(2, 3));
    ASSERT_TRUE(v1 != Vector(3, 3));

    v2 = Vector::Clone(v1);

    ASSERT_TRUE(v2 == v1) << "Clone Operation";

    v2.Set(5, 5);

    ASSERT_TRUE(v2 == Vector(5, 5)) << "Set Operation";
}
