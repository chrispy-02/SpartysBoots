/**
 * @file SensorRangeVisitor.cpp
 * @author Elijah Porter
 */

#include "pch.h"

#include "Product.h"
#include "SensorRangeVisitor.h"

/**
 * Visit a product and determine if it falls within the sensor's range.
 * @param product The product to be checked.
 */
void SensorRangeVisitor::VisitProduct(Product *product)
{
    if (product->WasKicked()) return;

    // The Y position in virtual pixels
    auto yPos = product->GetY();

    // If the position of the product is within the range
    if (yPos <= mMaxY && yPos >= mMinY)
    {
        mDetectedProduct = product;
    }
}
