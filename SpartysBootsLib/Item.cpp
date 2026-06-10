/**
 * @file Item.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include "Game.h"
#include "Item.h"


/**
 * Constructs a new Item.
 *
 * @param game The game this Item belongs to
 * @param filename The filename associated with the main image for this Item
 */
Item::Item(Game *game, const std::wstring &filename) : mGame(game)
{
    if (!filename.empty())
    {
        mImage = mGame->GetAssetManager().GetImage(filename);
    }
    else
    {
        mImage = nullptr;
    }
}

/**
 * Draws the Item to a given GC.
 *
 * @param graphics The GC to draw on
 */
void Item::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    if (mImage)
    {
        // Will only create a bitmap if the image doesn't already have one
        mImage->CreateBitmap(graphics);

        double wid = GetWidth();
        double hit = GetHeight();

        graphics->DrawBitmap(mImage->GetBitmap(), mPos.GetX() - wid / 2, mPos.GetY() - hit / 2, wid, hit);
    }
}

/**
 * Sets the target render dimensions of this Item.
 *
 * @param width The target height in virtual pixels
 * @param height The target width in virtual pixels
 */
void Item::SetTargetDimensions(double width, double height)
{
    double wid = mImage->GetWidth();
    double hit = mImage->GetHeight();

    mImageScale.Set(width / wid, height / hit);
}


/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The hit position in virtual pixels
 * @return Whether the mouse clicked this object
 */
bool Item::HitTest(Vector vec)
{
    // Get the width and height of the Item
    double wid = GetWidth();
    double hit = GetHeight();

    // Make the test point relative to the top-left corner of the Item
    double testX = vec.GetX() - mPos.GetX() + wid / 2;
    double testY = vec.GetY() - mPos.GetY() + hit / 2;

    // Test to see if x, y are in the Item
    if (testX < 0 || testY < 0 || testX >= wid || testY >= hit)
    {
        // We are outside the Item
        return false;
    }

    // If the object has no Image, or we are in the drawn part of the image, return true
    return (!mImage || !mImage->GetWxImage()->IsTransparent((int)testX, (int)testY));
}


/**
 * Load the attributes for an item node.
 *
 * This is the base class version that loads the attributes
 * common to all items. Override this to load custom attributes
 * for specific items.
 *
 * @param node The Xml node we are loading the item from
 */
void Item::XmlLoad(wxXmlNode *node) {}



/**
 * Reset position to initial starting position
 *
 * This will be called on all items every time the conveyor is started.
 * But we only want to do this for products, so Product will have an override
 */
void Item::Reset()
{
}