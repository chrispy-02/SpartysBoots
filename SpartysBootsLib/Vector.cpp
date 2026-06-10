/**
 * @file Vector.cpp
 * @author Elijah Porter
 */

#include "pch.h"

#include <cmath>

#include "Vector.h"

/**
 * Gets the magnitude (distance to origin) of the Vector
 *
 * @return The magnitude
 */
double Vector::GetMagnitude() const
{
    return std::sqrt(mX * mX + mY * mY);
}

/**
 * Computes the distance between this Vector and another Vector
 *
 * @param other The other Vector
 * @return The distance between them
 */
double Vector::DistanceFrom(const Vector& other) const
{
    auto dX = mX - other.mX;
    auto dY = mY - other.mY;

    return std::sqrt(dX * dX + dY * dY);
}

/**
 * Normalizes this Vector (sets the magnitude to 1 while keeping the correct direction).
 *
 * @return Reference to this iterator
 */
Vector& Vector::Normalize()
{
    auto magnitude = GetMagnitude();

    if (magnitude == 0)
    {
        return *this;
    }

    mX /= magnitude;
    mY /= magnitude;

    return *this;
}

/**
 * Sets the x and y coordinates for this Vector
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return Reference to this iterator
 */
Vector& Vector::Set(double x, double y)
{
    mX = x;
    mY = y;

    return *this;
}

/**
 * Copies a given Vector's values into a new Vector
 *
 * @param other The other Vector
 * @return The new Vector
 */
Vector Vector::Clone(const Vector& other)
{
    return Vector(other.GetX(), other.GetY());
}

/**
 * Compute the sum of this Vector and another Vector.
 *
 * @param other Another Vector
 * @return The resulting Vector
 */
Vector Vector::operator+(const Vector& other) const
{
    return Vector(mX + other.mX, mY + other.mY);
}

/**
 * Compute the difference between this Vector and another Vector.
 *
 * @param other Another Vector
 * @return The resulting Vector
 */
Vector Vector::operator-(const Vector& other) const
{
    return Vector(mX - other.mX, mY - other.mY);
}

/**
 * Returns a negated (flipped) version of this Vector
 *
 * @return The flipped Vector
 */
Vector Vector::operator-() const
{
    return Vector(-mX, -mY);
}

/**
 * Compute a scalar multiple of this Vector.
 *
 * @param scalar A scalar
 * @return The resulting Vector
 */
Vector Vector::operator*(const double scalar) const
{
    return Vector(mX * scalar, mY * scalar);
}

/**
 * Compute a scalar multiple of this Vector.
 *
 * @param scalar A scalar
 * @return The resulting Vector
 */
Vector Vector::operator/(const double scalar) const
{
    return Vector(mX / scalar, mY / scalar);
}

/**
 * Add another Vector to this Vector
 *
 * @param other The other Vector
 * @return Reference to this iterator
 */
Vector& Vector::operator+=(const Vector& other)
{
    mX += other.mX;
    mY += other.mY;

    return *this;
}

/**
 * Subtract another Vector from this Vector
 *
 * @param other The other Vector
 * @return Reference to this iterator
 */
Vector& Vector::operator-=(const Vector& other)
{
    mX -= other.mX;
    mY -= other.mY;

    return *this;
}

/**
 * Multiply this Vector by a scalar
 *
 * @param scalar The scalar
 * @return Reference to this iterator
 */
Vector& Vector::operator*=(const double scalar)
{
    mX *= scalar;
    mY *= scalar;

    return *this;
}

/**
 * Divide this Vector by a scalar
 *
 * @param scalar The scalar
 * @return Reference to this iterator
 */
Vector& Vector::operator/=(const double scalar)
{
    mX /= scalar;
    mY /= scalar;

    return *this;
}

/**
 * Checks if two Vector objects have the same value.
 *
 * @param other The other Vector
 * @return Whether they are equal
 */
bool Vector::operator==(const Vector& other) const
{
    return mX == other.mX && mY == other.mY;
}

/**
 * Checks if two Vector objects are different.
 *
 * @param other The other Vector
 * @return Whether they are not equal
 */
bool Vector::operator!=(const Vector& other) const
{
    return !(*this == other);
}
