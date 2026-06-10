/**
 * @file GateOr.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * An OR Gate for SpartysBoots.
 */

#pragma once

#include "Gate.h"
#include "Pin.h"

class Pin;

/**
 * The OR Gate class
 */
class GateOr : public Gate
{
  private:
    /// The first input for the OR Gate
    Pin mInputPin1 = Pin(PinType::Input, this);

    /// The second input for the OR Gate
    Pin mInputPin2 = Pin(PinType::Input, this);

    /// The output pin for the Gate
    Pin mOutputPin = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateOr() = delete;

    /// Copy constructor (disabled)
    GateOr(const GateOr &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateOr &) = delete;

    GateOr(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
};
