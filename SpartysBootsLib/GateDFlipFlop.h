/**
 * @file GateDFlipFlop.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The D Flip Flop Gate class
 */

#pragma once

#include "Gate.h"

/**
 * The D Flip Flop Gate class
 */
class GateDFlipFlop : public Gate
{
  private:
    /// The input for the D pin
    Pin mInputPinD = Pin(PinType::Input, this);

    /// The input for the Clock pin
    Pin mInputPinClock = Pin(PinType::Input, this);

    /// The output for the Q pin
    Pin mOutputPinQ = Pin(PinType::Output, this);

    /// The output for the Q' pin
    Pin mOutputPinQPrime = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateDFlipFlop() = delete;

    /// Copy constructor (disabled)
    GateDFlipFlop(const GateDFlipFlop &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateDFlipFlop &) = delete;

    GateDFlipFlop(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
    void Reset() override;
};
