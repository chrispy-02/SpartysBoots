/**
 * @file GateAnd.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The AND Gate class for SpartysBoots
 */

#pragma once

#include "Gate.h"

class Pin;

/**
 * The AND Gate class
 */
class GateAnd : public Gate
{
  private:
    /// The first input for the AND Gate
    Pin mInputPin1 = Pin(PinType::Input, this);

    /// The second input for the AND Gate
    Pin mInputPin2 = Pin(PinType::Input, this);

    /// The output pin for the Gate
    Pin mOutputPin = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateAnd() = delete;

    /// Copy constructor (disabled)
    GateAnd(const GateAnd &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateAnd &) = delete;

    GateAnd(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
};
