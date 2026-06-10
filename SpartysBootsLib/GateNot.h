/**
 * @file GateNot.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The NOT Gate class for SpartysBoots
 */

#pragma once

#include "Gate.h"
#include "Pin.h"

class Pin;

/**
 * The NOT Gate class
 */
class GateNot : public Gate
{
  private:
    /// The input for the NOT Gate
    Pin mInputPin = Pin(PinType::Input, this);

    /// The output pin for the Gate
    Pin mOutputPin = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateNot() = delete;

    /// Copy constructor (disabled)
    GateNot(const GateNot &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateNot &) = delete;

    GateNot(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
};
