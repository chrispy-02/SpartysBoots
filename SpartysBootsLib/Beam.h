/**
 * @file Beam.h
 * @author Zaid Qourah
 * @author Bob Wilson
 *
 * The beam class for detecting the presence of passing products.
 */

#pragma once

#include <memory>

#include "Gate.h"
#include "Item.h"
#include "Pin.h"

/// The length of the Beam's pin
const int BeamPinLength = 80;

/**
 * Class for a Beam
 */
class Beam : public Item, public PinOwner
{
  private:
    /// The output pin for the Beam
    Pin mOutputPin = Pin(PinType::Output, this, BeamPinLength);

    /// Position of the beam sender
    Vector mSenderPosition;

    /// Whether the beam is broken or not
    bool mIsBeamBroken = false;

    /// Image mirroring flag for the sender
    bool mMirrorSender;

    /// Whether the Gate is currently computing an output, used for cycle detection.
    bool mIsComputing = false;

  public:
    /// Default constructor (disabled)
    Beam() = delete;

    /// Copy constructor (disabled)
    Beam(const Beam &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Beam &) = delete;

    Beam(Game *game, bool mirrorSender = false);

    void XmlLoad(wxXmlNode *node) override;

    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    void Update(double elapsed) override;

    void DrawImage(std::shared_ptr<wxGraphicsContext> graphics, std::shared_ptr<Image> image, const Vector &position,
                   bool mirror);


    void Dragged(Vector mousePos) override;

    void SetCurrentPin(Pin *pin);

    /**
     * Gets the pin that is currently being dragged
     *
     * @return The current pin being dragged
     */
    Pin *GetCurrentPin() { return GetGame()->GetCurrentPin(); }

    bool HitTest(Vector vec) override;

    void ComputeState() override;
};
