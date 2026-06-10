/**
 * @file GateNot.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include "GateNot.h"
#include "Pin.h"

/// Size of the NOT Gate in virtual pixels
/// @return None
const wxSize GateNotSize(50, 50);

/// The divisor that determines radius of the NOT circle on the NOT Gate
const int NotCircleRadiusDivisor = 12;

/// Horizontal offset for output pins
const int OutputPinXOffset = 60;

/**
 * The NOT Gate constructor.
 *
 * @param game The game this Gate belongs to
 */
GateNot::GateNot(Game *game) : Gate(game, GateNotSize.GetWidth(), GateNotSize.GetHeight()) {}

/**
 * Updates the positions of the input and output pins for the NOT Gate
 */
void GateNot::UpdatePinPositions()
{
    auto width = GetWidth();  // Width of the NOT gate


    // Set the position of the input pin (on the left) and output pin (on the right)
    mInputPin.SetPosition(GetPosition() - Vector(width / 2, 0));      // Input pin (left-center)
    mOutputPin.SetPosition(GetPosition() - Vector(width / 2, 0) + Vector(OutputPinXOffset, 0)); // Output pin (Right center)
}

/**
 * Draws the GateNot on a given Graphics Context
 *
 * @param graphics The Graphics Context to draw on
 */
void GateNot::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Update pin positions (so that pins are drawn in the correct locations)
    UpdatePinPositions();

    // Draw the input and output pins
    mInputPin.Draw(graphics);  // Draw the input pin
    mOutputPin.Draw(graphics); // Draw the output pin

    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();
    auto path2 = graphics->CreatePath();

    // The location and size
    auto x = GetX();      // X position of the NOT gate
    auto y = GetY();      // Y position of the NOT gate
    auto w = GetWidth();  // Width of the NOT gate
    auto h = GetHeight(); // Height of the NOT gate

    // The three corner points of a NOT Gate
    wxPoint2DDouble p1(x - w / 2, y + h / 2);    // Bottom left
    wxPoint2DDouble p2(x + w / 2, y);            // Center right
    wxPoint2DDouble p3(x - w / 2, y - h / 2);    // Top left

    // Radius for the NOT circle at the output of the NOT gate
    auto rad = h / NotCircleRadiusDivisor;
    wxPoint2DDouble nc(x + (w / 2) + rad, y);    // Position of the NOT circle (right of the gate)

    // Create the path for the NOT gate (triangle shape)
    path.MoveToPoint(p1);               // Start at the bottom left
    path.AddLineToPoint(p2);            // Draw line to the center right
    path.AddLineToPoint(p3);            // Draw line to the top left
    path.CloseSubpath();                // Close the shape

    // Create the path for the NOT circle
    path2.AddArc(nc, rad, 0, 2 * M_PI, true); // Draw the small NOT circle
    path2.CloseSubpath();              // Close the circle

    // Draw the path (gate shape)
    graphics->SetPen(*wxBLACK_PEN);     // Set pen color to black
    graphics->SetBrush(*wxWHITE_BRUSH); // Set brush color to white (for filling the gate shape)
    graphics->DrawPath(path);           // Draw the NOT gate shape
    graphics->DrawPath(path2);          // Draw the NOT circle
}

/**
 * Sets an input if the wire end position is over the input pin.
 *
 * @param wirePos The wire end position in virtual pixels
 * @param wireOwner The Gate that wants to connect to an input
 */
void GateNot::OnReleaseWire(Vector wirePos, Pin *wireOwner)
{
    if (mInputPin.HitTest(wirePos))
    {
        mInputPin.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }
}

/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool GateNot::HitTest(Vector vec)
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
 * Computes the state for the GateNot's output pin
 */
void GateNot::ComputeState()
{
    // Prevent cycles from crashing the program
    if (GetIsComputing())
    {
        mOutputPin.SetState(LogicState::Unknown);
        return;
    }

    SetIsComputing(true);

    auto state = mInputPin.ComputeState();

    SetIsComputing(false);

    if (state == LogicState::Unknown)
    {
        mOutputPin.SetState(LogicState::Unknown);
        return;
    }

    // The NOT gate inverts the input state
    if (state == LogicState::One)
    {
        mOutputPin.SetState(LogicState::Zero);
        return;
    }

    mOutputPin.SetState(LogicState::One);
}
