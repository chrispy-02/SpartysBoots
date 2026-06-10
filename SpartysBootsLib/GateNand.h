/**
 * @file GateNand.h
 * @author Elijah Porter
 * @author Bob Wilson
 *
 * The NAND Gate class for SpartysBoots
 */

#pragma once

#include "Gate.h"
#include "Pin.h"

class Pin;

/**
 * The NAND Gate class
 */
class GateNand : public Gate
{
  private:
    /// The first input for the NAND Gate
    Pin mInputPin1 = Pin(PinType::Input, this);

    /// The second input for the NAND Gate
    Pin mInputPin2 = Pin(PinType::Input, this);

    /// The output pin for the Gate
    Pin mOutputPin = Pin(PinType::Output, this);

  public:
    /// Default constructor (disabled)
    GateNand() = delete;

    /// Copy constructor (disabled)
    GateNand(const GateNand &) = delete;

    /// Assignment operator (disabled)
    void operator=(const GateNand &) = delete;

    GateNand(Game *game);

    void ComputeState() override;
    void OnReleaseWire(Vector wirePos, Pin *wireOwner) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
    void UpdatePinPositions() override;
    bool HitTest(Vector vec) override;
};
