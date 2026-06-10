/**
 * @file GateNand.cpp
 * @author Elijah Porter
 */

#include "pch.h"

#include "GateNand.h"
#include "Pin.h"

/// Size of the NAND Gate in virtual pixels
/// @return None
const wxSize GateNandSize(75, 50);

/// The divisor that determines radius of the NOT circle on the NAND Gate
const int NotCircleRadiusDivisor = 12;

/**
 * The NAND Gate constructor.
 *
 * @param game The game this Gate belongs to
 */
GateNand::GateNand(Game *game) : Gate(game, GateNandSize.GetWidth(), GateNandSize.GetHeight()) {}

/**
 * Updates the positions of the children pins for the NAND Gate
 */
void GateNand::UpdatePinPositions()
{
    auto w = GetWidth();  // Width of the NAND gate

    // Adjust the pin positions based on the size of the gate and set the input/output pins correctly
    double pinOffsetX = 0; // Horizontal offset for input pins
    double pinOffsetY = 12; // Vertical offset for input pins

    double outputpinOffsetX = 84; // Horizontal offset for input pins


    // Adjust the positions for the input and output pins relative to the gate's position
    mInputPin1.SetPosition(GetPosition() - Vector(w / 2, 0) + Vector(pinOffsetX, -pinOffsetY)); // Input pin 1 (Top left)
    mInputPin2.SetPosition(GetPosition() - Vector(w / 2, 0) + Vector(pinOffsetX, pinOffsetY));  // Input pin 2 (Bottom left)
    mOutputPin.SetPosition(GetPosition() - Vector(w / 2, 0) + Vector(outputpinOffsetX, 0)); // Output pin (Right center)
}

/**
 * Draws the GateNand on a given Graphics Context
 *
 * @param graphics The Graphics Context to draw on
 */
void GateNand::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Update pin positions (so that pins are drawn in the correct locations)
    UpdatePinPositions();

    // Draw the input and output pins
    mInputPin1.Draw(graphics);  // Draw the first input pin
    mInputPin2.Draw(graphics);  // Draw the second input pin
    mOutputPin.Draw(graphics);  // Draw the output pin

    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();
    auto path2 = graphics->CreatePath();

    // The location and size
    auto x = GetX();      // X position of the NAND gate
    auto y = GetY();      // Y position of the NAND gate
    auto w = GetWidth();  // Width of the NAND gate
    auto h = GetHeight(); // Height of the NAND gate

    // The three corner points of a NAND gate
    wxPoint2DDouble bl(x - w / 2, y + h / 2);    // Bottom left
    wxPoint2DDouble cm(x + (w / 2) - (h / 2), y); // Circle center
    wxPoint2DDouble tl(x - w / 2, y - h / 2);    // Top left
    wxPoint2DDouble tr(x, y - h / 2);            // Top middle
    wxPoint2DDouble br(x, y + h / 2);            // Bottom middle

    // Radius for the NOT circle at the output of the NAND gate
    auto rad = h / NotCircleRadiusDivisor;
    wxPoint2DDouble nc(x + (w / 2) + rad, y); // Position of the NOT circle (right of the gate)

    // Create the path for the NAND gate (straight lines and arc)
    path.MoveToPoint(br);               // Start at the bottom middle
    path.AddLineToPoint(bl);            // Draw line to the bottom left
    path.AddLineToPoint(tl);            // Draw line to the top left
    path.AddLineToPoint(tr);            // Draw line to the top middle
    path.AddArc(cm, h / 2, -M_PI / 2, M_PI / 2, true); // Draw a semi-circular arc on the right
    path.CloseSubpath();                // Close the shape

    // Create the path for the NOT circle
    path2.AddArc(nc, rad, 0, 2 * M_PI, true); // Draw the small NOT circle
    path2.CloseSubpath();              // Close the circle

    // Draw the path (gate shape)
    graphics->SetPen(*wxBLACK_PEN);     // Set pen color to black
    graphics->SetBrush(*wxWHITE_BRUSH); // Set brush color to white (for filling the gate shape)
    graphics->DrawPath(path);           // Draw the NAND gate shape
    graphics->DrawPath(path2);          // Draw the NOT circle
}

/**
 * Sets an input if the wire end position is over an input pin.
 *
 * @param wirePos The wire end position in virtual pixels
 * @param wireOwner The Gate that wants to connect to an input
 */
void GateNand::OnReleaseWire(Vector wirePos, Pin *wireOwner)
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
bool GateNand::HitTest(Vector vec)
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
 * Computes the state for the GateNand's output pin
 */
void GateNand::ComputeState()
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

    if (state1 == LogicState::Unknown || state2 == LogicState::Unknown)
    {
        mOutputPin.SetState(LogicState::Unknown);
        return;
    }

    // If both inputs are 1, output 0
    if (state1 == LogicState::One && state2 == LogicState::One)
    {
        mOutputPin.SetState(LogicState::Zero);
        return;
    }

    // Otherwise output 1
    mOutputPin.SetState(LogicState::One);
}
