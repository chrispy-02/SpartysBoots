/**
 * @file LastProcessedVisitor.h
 * @author Elijah Porter
 *
 * A Product visitor that ensures the last Product has been processed.
 */

#pragma once

#include "ItemVisitor.h"
#include "Product.h"

/**
 * The Product visitor class that ensures that checks if last Product has been processed.
 */
class LastProcessedVisitor : public ItemVisitor
{
  private:
    /// Whether the last Product has been processed
    bool mLastProcessed = false;

  public:
    /**
     * The constructor for this visitor
     */
    LastProcessedVisitor() {}

    /**
     * Gets whether the last product has been processed
     * @return Whether the last product has been processed
     */
    bool HasLastBeenProcessed() { return mLastProcessed; }

    /**
     * Visits a product and checks whether it is the last product, if it is then it checks if it has been processed.
     * @param product The product to visit
     */
    void VisitProduct(Product *product) override
    {
        // only change mLastProcessed if it is not already set true
        mLastProcessed = !mLastProcessed ? product->GetIsLast() && product->IsProcessed() : true;
    }
};
