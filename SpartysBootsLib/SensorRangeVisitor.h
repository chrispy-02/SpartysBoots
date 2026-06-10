/**
 * @file SensorRangeVisitor.h
 * @author Elijah Porter
 *
 * A Visitor that visits products and finds one in the sensors range
 */

#pragma once

#include "ItemVisitor.h"

class Product;

/**
 * A Visitor class that visits products and finds one in the sensors range
 */
class SensorRangeVisitor : public ItemVisitor
{
  private:
    /// The last product to be detected by the sensor
    Product *mDetectedProduct = nullptr;

    /// The minimum Y position of a product
    double mMinY = 0;

    /// The maximum Y position of a product
    double mMaxY = 0;

  public:
    /**
     * Visitor Constructor
     *
     * @param minY The minimum Y value in virtual pixels
     * @param maxY The maximum Y value in virtual pixels
     */
    SensorRangeVisitor(double minY, double maxY) : mMinY(minY), mMaxY(maxY) {}

    /**
     * Returns the detected Product within the sensor range
     * @return The detected Product
     */
    Product * GetDetectedProduct() { return mDetectedProduct; }

    void VisitProduct(Product *product) override;
};
