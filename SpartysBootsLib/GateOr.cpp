/**
 * @file GateOr.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include "GateOr.h"
#include "Pin.h"

/// Size of the OR Gate in virtual pixels
/// @return None
const wxSize GateOrSize(75, 50);

/// The offset for the input pins
const Vector PinOffset(12, 12);

/**
 * The OR Gate constructor.
 *
 * @param game The game this Gate belongs to
 */
GateOr::GateOr(Game *game) : Gate(game, GateOrSize.GetWidth(), GateOrSize.GetHeight()) {}

/**
 * Updates the positions of the children pins
 */
void GateOr::UpdatePinPositions()
{
    auto w = GetWidth();

    mInputPin1.SetPosition(GetPosition() - Vector(w / 2, 0) + PinOffset.FlippedY());
    mInputPin2.SetPosition(GetPosition() - Vector(w / 2, 0) + PinOffset);
    mOutputPin.SetPosition(GetPosition() + Vector(w / 2, 0));
}


/**
 * Draws the GateOr on a given Graphics Context
 *
 * @param graphics The Graphics Context to draw on
 */
void GateOr::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Update pin positions (so that pins are drawn in the correct locations)
    UpdatePinPositions();

    // Draw the input and output pins
    mInputPin1.Draw(graphics);  // Draw the first input pin
    mInputPin2.Draw(graphics);  // Draw the second input pin
    mOutputPin.Draw(graphics);  // Draw the output pin

    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();

    // The location and size
    auto x = GetX();
    auto y = GetY();
    auto w = GetWidth();
    auto h = GetHeight();

    // The three corner points of an OR gate
    wxPoint2DDouble p1(x - w / 2, y + h / 2); // Bottom left
    wxPoint2DDouble p2(x + w / 2, y); // Center right
    wxPoint2DDouble p3(x - w / 2, y - h / 2); // Top left

    // Control points used to create the Bezier curves
    auto controlPointOffset1 = wxPoint2DDouble(w * 0.5, 0);
    auto controlPointOffset2 = wxPoint2DDouble(w * 0.75, 0);
    auto controlPointOffset3 = wxPoint2DDouble(w * 0.2, 0);

    // Create the path for the gate
    path.MoveToPoint(p1);
    path.AddCurveToPoint(p1 + controlPointOffset1, p1 + controlPointOffset2, p2);
    path.AddCurveToPoint(p3 + controlPointOffset2, p3 + controlPointOffset1, p3);
    path.AddCurveToPoint(p3 + controlPointOffset3, p1 + controlPointOffset3, p1);
    path.CloseSubpath();

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawPath(path);
}

/**
 * Sets an input if the wire end position is over an input pin.
 *
 * @param wirePos The wire end position in virtual pixels
 * @param wireOwner The Gate that wants to connect to an input
 */
void GateOr::OnReleaseWire(Vector wirePos, Pin *wireOwner)
{
    if (mInputPin1.HitTest(wirePos))
    {
        mInputPin1.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }

    else if (mInputPin2.HitTest(wirePos))
    {
        mInputPin2.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }
}

/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool GateOr::HitTest(Vector vec)
{
    // If output pin was clicked, start dragging a wire from it
    if (mOutputPin.HitTest(vec))
    {
        mOutputPin.SetWireEnd(vec);
        SetCurrentPin(&mOutputPin);
        return true;
    }

    // Upcall to find out if clicked the object
    return Item::HitTest(vec);
}


/**
 * Computes the state for the GateOr's output pin
 */
void GateOr::ComputeState()
{
    // Prevent cycles from crashing the program
    if (GetIsComputing())
    {
        mOutputPin.SetState(LogicState::Unknown);
        return;
    }

    SetIsComputing(true);

    auto state1 = mInputPin1.ComputeState();
    auto state2 = mInputPin2.ComputeState();

    SetIsComputing(false);

    // If either input computes to an unknown state, the output is unknown
    if (state1 == LogicState::Unknown || state2 == LogicState::Unknown)
    {
        mOutputPin.SetState(LogicState::Unknown);
        return;
    }

    // If either input computes to the One state, return the One state. Otherwise, return Zero.
    if (state1 == LogicState::One || state2 == LogicState::One)
    {
        mOutputPin.SetState(LogicState::One);
        return;
    }

    mOutputPin.SetState(LogicState::Zero);
}
