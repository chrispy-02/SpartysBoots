/**
 * @file Sparty.h
 * @author Christopher Nguyen
 * @author Elijah Porter
 *
 * Class that implements Sparty, responsible for kicking products off the conveyor belt.
 */

#pragma once

#include "Item.h"

#include <memory>
#include <wx/graphics.h>

/**
 * Class that represents Sparty.
 */
class Sparty : public Item, public PinOwner
{
  private:
    /// Image for the Sparty background (behind the boot)
    std::shared_ptr<Image> mBackImage;

    /// Image for the Sparty boot (the leg that rotates)
    std::shared_ptr<Image> mBootImage;

    /// Image for the Sparty foreground (in front of the boot)
    std::shared_ptr<Image> mFrontImage;

    /// The pivot point for the boot rotation as a fraction of width and height
    const wxPoint2DDouble mBootPivot = wxPoint2DDouble(0.5, 0.55);

    /// The maximum rotation angle for the boot in radians
    const double MaxBootRotation = 0.8;

    /// The percentage of the height where the boot tip is located
    const double BootTipPercentage = 0.80;

    /// The duration of the kick animation in seconds
    double mKickDuration = 1;

    /// The elapsed time since the kick started
    double mKickTime = 0.0;

    /// Flag to indicate if Sparty is currently kicking
    bool mIsKicking = false;

    /// The Sparty input pin
    Pin mInputPin = Pin(PinType::Input, this);

    /// Calculated width of Sparty
    double mWidth = 0.0;

    /// Calculated height of Sparty
    double mHeight = 0.0;

    /// The last state that Sparty received
    LogicState mLastState = LogicState::Unknown;

  public:
    /// Default constructor (disabled)
    Sparty() = delete;

    /// Copy constructor (disabled)
    Sparty(const Sparty &) = delete;

    /// Assignment operator
    void operator=(const Sparty &) = delete;

    /// Constructor
    Sparty(Game *game);

    /// Load the attributes for an item node
    void XmlLoad(wxXmlNode *node) override;

    /// Draw Sparty
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /// Update Sparty's state
    void Update(double elapsed) override;

    double GetKickProgress();

    /**
     * Gets the width of Sparty
     * @return The width of Sparty
     */
    double GetWidth() override { return mWidth; }

    /**
     * Gets the height of Sparty
     * @return The height of Sparty
     */
    double GetHeight() override { return mHeight; }

    /**
     * Computes the current state of the Sparty pin
     */
    void ComputeState() override {}

    void Released(Vector vec) override;

    void SetCurrentPin(Pin *pin);

    /**
     * Mouse click hit test on a Sparty
     *
     * @param vec The hit position in virtual pixels
     * @return False because a Sparty cannot be clicked on
     */
    bool HitTest(Vector vec) override { return false; }
};
