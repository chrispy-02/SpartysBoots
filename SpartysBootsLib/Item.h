/**
 * @file Item.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The base Item class
 */

#pragma once

#include <wx/graphics.h>
#include <wx/image.h>

#include "Game.h"
#include "Image.h"
#include "ItemVisitor.h"
#include "Vector.h"


// This is a forward reference to avoid a circular dependency.
// Items need to know the game they are a member of,
// But the game needs to know about the items in order to draw them.
// When you use a forward reference, you still have to include the header in the .cpp file
class Game;

/// The Red color
/// @return None
const wxColour ColorRed(187, 0, 0);

/// The Green color
/// @return None
const wxColour ColorGreen(24, 69, 59);

/// The Blue color
/// @return None
const wxColour ColorBlue(0, 39, 76);

/// The Gray color
/// @return None
const wxColour ColorGray(128, 128, 128);

/// The Light Gray color
/// @return None
const wxColour ColorLightGray(200, 200, 200);

/// The Less Light Gray color
/// @return None
const wxColour ColorLessLightGray(180, 180, 180);

/**
 * Base class for any item in our game.
 */
class Item
{
  private:
    /// Item location in the game stored as a vector
    Vector mPos;

    /// The image scale to apply
    Vector mImageScale = Vector(1, 1);

    /// The main image associated with this Item
    std::shared_ptr<Image> mImage;

    /// The game this item belongs to
    Game *mGame;

  public:

    /**
     * Virtual destructor
    */
    ~Item() {}

    /// Default constructor (disabled)
    Item() = delete;

    /// Copy constructor (disabled)
    Item(const Item &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Item &) = delete;

    Item(Game *game, const std::wstring &filename);

    /**
     * Gets the game this item belongs to
     *
     * @return The game this item belongs to
     */
    Game *GetGame() { return mGame; }

    /**
     * Gets the image this item has
     *
     * @return The game this item belongs to
     */
    std::shared_ptr<Image> GetImage() { return mImage; }

    /**
     * Gets the Vector position of this item (in virtual pixels)
     *
     * @return The position of this item (in virtual pixels)
     */
    Vector GetPosition() { return mPos; }

    /**
     * Gets the X position of this item (in virtual pixels)
     *
     * @return The X position of this item (in virtual pixels)
     */
    double GetX() { return mPos.GetX(); }

    /**
     * Gets the Y position of this item (in virtual pixels)
     *
     * @return The Y position of this item (in virtual pixels)
     */
    double GetY() { return mPos.GetY(); }

    /**
     * Sets the X position of this item (in virtual pixels)
     *
     * @param x The x value to set it to (in virtual pixels)
     */
    void SetX(double x) { mPos.SetX(x); }

    /**
     * Sets the Y position of this item (in virtual pixels)
     *
     * @param y The y value to set it to (in virtual pixels)
     */
    void SetY(double y) { mPos.SetY(y); }

    /**
     * Gets the width of this Item in virtual pixels (includes the scale).
     *
     * @return The width of this Item in virtual pixels.
     */
    virtual double GetWidth() { return mImage->GetWidth() * mImageScale.GetX(); }

    /**
     * Gets the height of this Item in virtual pixels (includes the scale).
     *
     * @return The height of this Item in virtual pixels.
     */
    virtual double GetHeight() { return mImage->GetHeight() * mImageScale.GetY(); }

    /**
     * This function is called when the Item is Dragged.
     *
     * @param mousePos The mouse position in virtual pixels
     */
    virtual void Dragged(Vector mousePos) {}

    /**
     * This function is called when the Item is released.
     *
     * @param mousePos The mouse position in virtual pixels
     */
    virtual void Released(Vector mousePos) {}

    /**
     * Sets the Vector position of this item (in virtual pixels)
     *
     * @param vec The position (in virtual pixels)
     */
    void SetPosition(Vector vec) { mPos = vec; }

    void SetTargetDimensions(double width, double height);

    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    virtual bool HitTest(Vector vec);

    virtual void XmlLoad(wxXmlNode *node);

    virtual void Reset();

    /**
     * Accepts a visitor
     * @param visitor The visitor to accept
     */
    virtual void AcceptVisitor(ItemVisitor *visitor) { visitor->VisitItem(this); }

    /**
     * Handle updates for animation
     * @param elapsed The time since the last update
     */
    virtual void Update(double elapsed) {}

    /**
     * Test Item to see if crossing conveyor
     *
     * We only need to do this for Products, so Product will override
     * @param beamY The Y value of beam in virtual pixels
     * @return whether the Item is crossing the beam
     */
    virtual bool IsCrossingBeam(double beamY) { return false; }
};
