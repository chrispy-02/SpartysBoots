/**
 * @file ScoringVisitor.cpp
 * @author Christopher Nguyen
 */

#include "pch.h"
#include "ScoringVisitor.h"

ScoringVisitor::ScoringVisitor(Game* game) : mGame(game) {}

void ScoringVisitor::VisitProduct(Product* product)
{
    if (product->HasBeenScored())
    {
        // Skip products that have already been scored
        return;
    }

    if (product->WasKicked())
    {
        // Product has been kicked
        if (product->ShouldBeKicked())
        {
            // Correctly kicked
            mGame->AddToLevelScore(mGame->GetGoodScore());
        }
        else
        {
            // Incorrectly kicked
            mGame->AddToLevelScore(mGame->GetBadScore());
        }

        product->SetHasBeenScored(true);
    }
    else if (product->HasPassedBeam())
    {
        // Product was not kicked but has passed the beam
        if (product->ShouldBeKicked())
        {
            // Should have been kicked but wasn't
            mGame->AddToLevelScore(mGame->GetBadScore());
            product->SetHasBeenScored(true);
        }
        else
        {
            // Correctly not kicked
            mGame->AddToLevelScore(mGame->GetGoodScore());
            product->SetHasBeenScored(true);
        }
    }
}
