/**
 * @file GateSRFlipFlop.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The SR Flip Flop Gate class
 */

#pragma once

#include "Gate.h"
#include "Pin.h"

/**
 * The SR Flip Flop Gate class
 */
class GateSRFlipFlop : public Gate
{
  private:
    /// The input for the S pin
    Pin mInputPinS = Pin(PinType::Input, this);

    /// The input for the R pin
    Pin mInputPinR = Pin(PinType::Input, this);

    /// The output for the Q pin
    Pin mOutputPinQ = Pin(PinType::Output, this);

    /// The output for the Q' pin
    Pin mOutputPinQPrime = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateSRFlipFlop() = delete;

    /// Copy constructor (disabled)
    GateSRFlipFlop(const GateSRFlipFlop &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateSRFlipFlop &) = delete;

    GateSRFlipFlop(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
    void Reset() override;
};
