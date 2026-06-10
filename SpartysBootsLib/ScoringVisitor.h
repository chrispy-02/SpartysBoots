/**
 * @file ScoringVisitor.h
 * @author Christopher Nguyen
 *
 *
 */

#ifndef SPARTYSBOOTS_SPARTYSBOOTSLIB_SCORINGVISITOR_H
#define SPARTYSBOOTS_SPARTYSBOOTSLIB_SCORINGVISITOR_H

#include "ItemVisitor.h"
#include "Product.h"
#include "Game.h"

/**
 * A visitor that applies scoring logic to products.
 */
class ScoringVisitor : public ItemVisitor
{
private:
    /// Pointer to the Game instance
    Game* mGame;

public:
    /**
     * Constructor
     * @param game Pointer to the Game instance
     */
    ScoringVisitor(Game* game);

    /**
     * Visit a product to apply scoring logic
     * @param product The product to visit
     */
    void VisitProduct(Product* product) override;
};

#endif //SPARTYSBOOTS_SPARTYSBOOTSLIB_SCORINGVISITOR_H
