/**
 * @file Pin.h
 * @author Zaid Qourah
 * @author Elijah Porter
 *
 * The Pin class, for interfacing with Gates, Sensors, and more
 */

#pragma once

#include <memory>

#include "LogicState.h"
#include "PinOwner.h"
#include "Vector.h"

/// Default length of line from the pin
static const int DefaultLineLength = 20;

/// Color to use for drawing an unknown state connection wire
const wxColour ConnectionColorUnknown = wxColour(128, 128, 128);

/// Color to use for drawing a zero connection wire
const wxColour ConnectionColorZero = *wxBLACK;

/// Color to use for drawing a one connection wire
const wxColour ConnectionColorOne = *wxRED;

/// Wire width for drawing wires between pins
static const int WireWidth = 3;

/**
 * Enum for the type of pin
 */
enum class PinType
{
    Input,
    Output
};

/**
 * Class for a Pin
 */
class Pin
{
  private:
    /// The type of pin (Input/Output)
    PinType mType;

    /// The position of the pin
    Vector mPosition;

    /// The length of the pin
    double mLength;

    /// The end of the connection wire
    Vector mWireEnd;

    /// The owner/parent of this pin
    PinOwner *mOwner = nullptr;

    /// The single pin that this pin is connected to
    Pin *mIncomingPin = nullptr;

    /// Whether the pin is actively dragging a wire
    bool mIsActive = false;

    /// Color of the pin
    wxColour mColor;

    /// State of the pin. This is currently only being used for output pins.
    LogicState mState = LogicState::Unknown;

  public:
    /**
     * Sets the position of the Pin
     * @param position The Pin's position in virtual pixels
     */
    void SetPosition(Vector position) { mPosition = position; }

    /**
     * Sets the end of the wire
     * @param position the end of the wire
     */
    void SetWireEnd(Vector position) { mWireEnd = position; }

    /**
     * Connects to a pin
     * @param pin The pin to connect to
     */
    void SetIncomingPin(Pin *pin) { mIncomingPin = pin; }

    /**
     * Sets whether this pin is actively dragging
     * @param active Whether this pin is dragging
     */
    void SetActive(bool active) { mIsActive = active; }

    /**
     * Constructor for a pin of a given type
     * @param type The type of this pin
     * @param owner The owner of this pin
     * @param length The length of the pin in virtual pixels (default: 20)
     */
    Pin(PinType type, PinOwner *owner, double length = DefaultLineLength) : mType(type), mOwner(owner), mLength(length)
    {
    }

    /**
     * Returns the color of the pin
     * @return The color of the pin
     */
    wxColour GetColor() { return mColor; }

    void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    /**
     * Gets the owner of the Pin
     * @return A pointer to the Pin's owner
     */
    PinOwner *GetPinOwner() { return mOwner; }


    /**
     * Gets the position of the Pin
     * @return The Pin's position in virtual pixels
     */
    Vector GetPosition() { return mPosition; }

    /**
     * Gets the length of the Pin
     * @return The Pin's position in virtual pixels
     */
    double GetLength() { return mLength; }


    bool HitTest(Vector vec);

    LogicState ComputeState();

    /**
     * Set the state of the pin. This is currently only being used for output pins.
     * @param state The state we are setting the pin to
     */
    void SetState(LogicState state) { mState = state; }

    /**
     * Get the current state of the pin, without computing any logic.
     *
     * This is currently only being used to get the previous state of the GateDFlipFlop Clock pin, to see if it changes
     * from 0 to 1.
     *
     * @return The state of the pin
     */
    LogicState GetState() { return mState; }
};
