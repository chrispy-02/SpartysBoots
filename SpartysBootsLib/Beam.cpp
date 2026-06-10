/**
 * @file Beam.cpp
 * @author Zaid Qourah
 * @author Bob Wilson
 */

#include "pch.h"

#include <iostream>
#include <wx/graphics.h>

#include "Beam.h"
#include "Pin.h"

using namespace std;

/// Image for the beam sender and receiver when red
const std::wstring BeamRedImage = L"beam-red.png";

/// Image for the beam sender and receiver when green
const std::wstring BeamGreenImage = L"beam-green.png";

/// The color for the beam's outerglow
/// @return A wxColour representing the outer glow color of the beam
const wxColour OuterGlowColor(255, 200, 200, 100);

/// The color of the laser beam
/// @return A wxColour representing the color of the laser beam
const wxColour LaserBeamColor(255, 0, 0, 175);

/// The thickness of the laser beam
const int LaserBeamThickness = 4;

/**
 * Constructor for the Beam
 * @param game The game instance
 * @param mirrorSender Whether to mirror the sender image
 */
Beam::Beam(Game *game, bool mirrorSender) : Item(game, BeamGreenImage), mMirrorSender(mirrorSender) {}

/**
 * Load the beam attributes from the XML node
 * @param node The XML node to load from
 */
void Beam::XmlLoad(wxXmlNode *node)
{
    double x = stod(node->GetAttribute(L"x").ToStdString());
    double y = stod(node->GetAttribute(L"y").ToStdString());
    SetPosition(Vector(x, y));
    mOutputPin.SetPosition(Vector(x, y));

    int senderOffset = stoi(node->GetAttribute(L"sender").ToStdString());
    mSenderPosition = Vector(x + senderOffset, y);

    // Store the beam's Y-coordinate in the Game
    GetGame()->SetBeamY(y);
}

/**
 * Update the beam's state
 * @param elapsed The time since the last update
 */
void Beam::Update(double elapsed)
{
    // Check the items to see if any are crossing the beam
    mIsBeamBroken = (GetGame()->BeamTest(GetY()) != nullptr);
}

/**
 * Draw the beam on the screen
 * @param graphics The graphics context to draw on
 */
void Beam::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    mOutputPin.Draw(graphics); // Draw the output pin

    std::wstring currentImage = mIsBeamBroken ? BeamRedImage : BeamGreenImage;

    // Offset the position to the left
    Vector receiverPosition = GetPosition() - Vector(GetWidth() / 2, GetHeight() / 2);
    Vector senderPosition = mSenderPosition - Vector(GetWidth() / 2, GetHeight() / 2);

    auto beamImage = GetGame()->GetAssetManager().GetImage(currentImage, graphics);

    // Draw the glowing laser beam
    wxPen laser1(OuterGlowColor, 2 * LaserBeamThickness); // Outer glow
    wxPen laser2(LaserBeamColor, LaserBeamThickness); // Inner laser beam

    // Outer glow line
    graphics->SetPen(laser1);
    graphics->StrokeLine(
        receiverPosition.GetX() + beamImage->GetWidth() / 2, receiverPosition.GetY() + beamImage->GetHeight() / 2,
        senderPosition.GetX() + beamImage->GetWidth() / 2, senderPosition.GetY() + beamImage->GetHeight() / 2);

    // Inner beam line
    graphics->SetPen(laser2);
    graphics->StrokeLine(
        receiverPosition.GetX() + beamImage->GetWidth() / 2, receiverPosition.GetY() + beamImage->GetHeight() / 2,
        senderPosition.GetX() + beamImage->GetWidth() / 2, senderPosition.GetY() + beamImage->GetHeight() / 2);

    // Draw the sender on the left, with mirrored image
    DrawImage(graphics, beamImage, receiverPosition, true);

    // Draw the receiver on the right
    DrawImage(graphics, beamImage, senderPosition, false);
}


/**
 * Helper function to draw an image with optional mirroring.
 * @param graphics The graphics context to draw on.
 * @param image The image to draw.
 * @param position The position where the image should be drawn.
 * @param mirror Whether the image should be mirrored or not.
 */
void Beam::DrawImage(std::shared_ptr<wxGraphicsContext> graphics, std::shared_ptr<Image> image, const Vector &position,
                     bool mirror)
{
    graphics->PushState(); // Save the current state

    if (mirror)
    {
        graphics->Scale(-1, 1);
        graphics->DrawBitmap(image->GetBitmap(),
                             -position.GetX() - image->GetWidth(), // Adjust for mirroring
                             position.GetY(), image->GetWidth(), image->GetHeight());
    }
    else
    {
        // Draw normally without mirroring
        graphics->DrawBitmap(image->GetBitmap(), position.GetX(), position.GetY(), image->GetWidth(),
                             image->GetHeight());
    }

    graphics->PopState(); // Restore the state after drawing
}


/**
 * If the user is dragging a wire, set the wire endpoint to the mouse position.
 *
 * @param mousePos The mouse position in virtual pixels
 */
void Beam::Dragged(Vector mousePos)
{
    auto currentPin = GetCurrentPin();

    if (currentPin != nullptr)
    {
        currentPin->SetWireEnd(mousePos);
    }
}


/**
 * Sets the current pin being dragged
 *
 * @param pin the pin currently being dragged
 */
void Beam::SetCurrentPin(Pin *pin)
{
    auto currentPin = GetCurrentPin();

    if (pin != nullptr)
    {
        pin->SetActive(true);
    }

    if (currentPin != nullptr)
    {
        currentPin->SetActive(false);
    }

    GetGame()->SetCurrentPin(pin);
}


/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool Beam::HitTest(Vector vec)
{
    // If output pin was clicked, start dragging a wire from it
    if (mOutputPin.HitTest(vec))
    {
        mOutputPin.SetWireEnd(vec);
        SetCurrentPin(&mOutputPin);
        return true;
    }

    return false;
}


/**
 * Computes the state for the Beam's output pin
 */
void Beam::ComputeState() { mOutputPin.SetState(mIsBeamBroken ? LogicState::One : LogicState::Zero); }

