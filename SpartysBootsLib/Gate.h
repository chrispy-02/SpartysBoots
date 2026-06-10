/**
 * @file Gate.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The Gate class for SpartysBoots. Part of the circuit that controls Sparty.
 */

#pragma once

#include "Item.h"
#include "PinOwner.h"

class Pin;

/**
 * The base Gate class for SpartysBoots.
 */
class Gate : public Item, public PinOwner
{
  private:
    /// The Gate width in virtual pixels
    double mWidth;

    /// The Gate height in virtual pixels
    double mHeight;

    /// Whether the Gate is currently computing an output, used for cycle detection.
    bool mIsComputing = false;

  public:
    /**
     * Virtual destructor
    */
    ~Gate() {}

    /// Default constructor (disabled)
    Gate() = delete;

    /// Copy constructor (disabled)
    Gate(const Gate &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Gate &) = delete;

    Gate(Game *game, double wid, double hit);

    /**
     * Returns the width of the Gate in virtual pixels.
     *
     * @return The width in virtual pixels
     */
    double GetWidth() override { return mWidth; }

    /**
     * Returns the height of the Gate in virtual pixels.
     *
     * @return The height in virtual pixels
     */
    double GetHeight() override { return mHeight; }

    void Dragged(Vector mousePos) override;

    void Released(Vector mousePos) override;


    /**
     * Sets whether this Gate is currently computing an output state.
     *
     * @param computing Whether this Gate is computing an output
     */
    void SetIsComputing(bool computing) { mIsComputing = computing; }

    void SetCurrentPin(Pin *pin);

    /**
     * Gets the pin that is currently being dragged
     *
     * @return The current pin being dragged
     */
    Pin *GetCurrentPin() { return GetGame()->GetCurrentPin(); }


    /**
     * Gets whether this Gate is computing an output state or not.
     *
     * @return Whether this Gate is computing an output
     */
    bool GetIsComputing() { return mIsComputing; }

    /**
     * Computes the state for each of the Gate's output pins
     * */
    void ComputeState() override = 0;

    /**
     * Draws this Gate to a given graphics context.
     *
     * @param graphics The graphics context
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override = 0;

    /**
     * Tests whether a given position is within this Gate.
     *
     * @param vec A position in virtual pixels
     * @return Whether that position lies within the Gate
     */
    bool HitTest(Vector vec) override = 0;

    /// Updates the positions of the children pins
    virtual void UpdatePinPositions() {}

    /**
     * Handle the release of a wire being dragged to potentially connect to an input pin.
     * @param wirePos The position where the wire was released.
     * @param wireOwner The output pin that was being dragged.
     */
    virtual void OnReleaseWire(Vector wirePos, Pin* wireOwner) = 0;
};
