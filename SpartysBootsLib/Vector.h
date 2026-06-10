/**
 * @file Vector.h
 * @author Elijah Porter
 *
 * This class represents a 2D Vector in virtual pixel space.
 */

#pragma once

/**
 * The Vector class for SpartysBoots, ir represents a 2D Vector in virtual pixel space.
 */
class Vector
{
  private:
    /// The x-coord of the Vector
    double mX;

    /// The y-coord of the Vector
    double mY;

  public:
    /**
     * Default constructor initializes the Vector to <0, 0>
     */
    Vector() : mX(0), mY(0) {}

    /**
     * This constructor initializes the Vector to <x, y>
     *
     * @param x The x-coord
     * @param y The y-coord
     */
    Vector(double x, double y) : mX(x), mY(y) {}

    static Vector Clone(const Vector &other);

    /**
     * Returns the X-coordinate of the Vector
     * @return The X-coordinate of the Vector
     */
    double GetX() const { return mX; }

    /**
     * Returns the Y-coordinate of the Vector
     * @return The Y-coordinate of the Vector
     */
    double GetY() const { return mY; }

    /**
     * Sets the X-coordinate of the Vector
     * @param x The Y value we are setting it to
     */
    void SetX(double x) { mX = x; }

    /**
     * Sets the Y-coordinate of the Vector
     * @param y The Y value we are setting it to
     */
    void SetY(double y) { mY = y; }

    double GetMagnitude() const;
    double DistanceFrom(const Vector &other) const;

    /**
     * Returns a new Vector that has the X-coordinate of the Vector flipped
     * @return the flipped Vector
     */
    Vector FlippedX() const { return Vector(-mX, mY); }

    /**
     * Returns a new Vector that has the Y-coordinate of the Vector flipped
     * @return the flipped Vector
     */
    Vector FlippedY() const { return Vector(mX, -mY); }

    Vector &Set(double x, double y);
    Vector &Normalize();

    Vector operator+(const Vector &other) const;
    Vector operator-(const Vector &other) const;
    Vector operator-() const;
    Vector operator*(double scalar) const;
    Vector operator/(double scalar) const;

    Vector &operator+=(const Vector &other);
    Vector &operator-=(const Vector &other);
    Vector &operator*=(double scalar);
    Vector &operator/=(double scalar);

    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;
};
