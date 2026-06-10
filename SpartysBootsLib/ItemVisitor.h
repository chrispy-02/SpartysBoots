/**
 * @file ItemVisitor.h
 * @author Elijah Porter
 *
 * The base class for the item visitor
 */

#pragma once

class Item;
class Product;

/**
 * The base class for all Item visitors
 */
class ItemVisitor
{
  protected:
    /**
     * Constructor
     * Ensures this is an abstract class
     */
    ItemVisitor() {}

  private:
  public:
    virtual ~ItemVisitor() {}

    /**
     * Visits a product
     * @param product The product to visit
     */
    virtual void VisitProduct(Product *product) {}

    /**
     * Visits an item
     * @param item The item to visit
     */
    virtual void VisitItem(Item *item) {}
};
