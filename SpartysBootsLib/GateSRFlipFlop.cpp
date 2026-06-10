/**
 * @file GateSRFlipFlop.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include "GateSRFlipFlop.h"
#include "Pin.h"

/**
 * Size of the SR Flip Flop Gate in virtual pixels
 * @return
*/
const wxSize SRFlipFlopSize(50, 75);

/// Margin for labels inside the flip flop
const int SRFlipFlopLabelMargin = 3;

/// The font size for this Gate
const int FontSize = 15;

/**
 * The SR Flip Flop Gate constructor.
 *
 * @param game The game this Gate belongs to
 */
GateSRFlipFlop::GateSRFlipFlop(Game *game)
    : Gate(game, SRFlipFlopSize.GetWidth(), SRFlipFlopSize.GetHeight())
{
    mOutputPinQ.SetState(LogicState::Zero);
    mOutputPinQPrime.SetState(LogicState::One);
}

/**
 * Updates the positions of the children pins
 */
void GateSRFlipFlop::UpdatePinPositions()
{
    auto width = GetWidth();
    auto halfHeight = GetHeight() / 2;

    // Set the position of the input pins (S and R)
    mInputPinS.SetPosition(GetPosition() - Vector(width / 2, halfHeight / 2)); // S input (Top left)
    mInputPinR.SetPosition(GetPosition() - Vector(width / 2, -halfHeight / 2)); // R input (Bottom left)

    // Set the position of the output pins (Q and Q')
    mOutputPinQ.SetPosition(GetPosition() + Vector(width / 2, -halfHeight / 2));  // Q output (Top right)
    mOutputPinQPrime.SetPosition(GetPosition() + Vector(width / 2, halfHeight / 2));  // Q' output (Bottom right)
}

/**
 * Draws the GateSRFlipFlop on a given Graphics Context
 *
 * @param graphics The Graphics Context to draw on
 */
void GateSRFlipFlop::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Update pin positions
    UpdatePinPositions();

    // Draw the pins
    mInputPinS.Draw(graphics);  // Draw S input pin
    mInputPinR.Draw(graphics);  // Draw R input pin

    // Draw Q (black) and Q' (red) pins using their respective colors
    mOutputPinQ.Draw(graphics);  // Draw Q output pin (black)
    mOutputPinQPrime.Draw(graphics);  // Draw Q' output pin (red)

    // Draw the rectangle representing the gate
    auto x = GetX();
    auto y = GetY();
    auto w = GetWidth();
    auto h = GetHeight();

    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawRectangle(x - w / 2, y - h / 2, w, h);

    // Draw labels (S, R, Q, Q')
    auto font = graphics->CreateFont(FontSize, L"Arial", wxFONTFLAG_BOLD, *wxBLACK);
    graphics->SetFont(font);

    double letterWidth, letterHeight;
    graphics->GetTextExtent(L"S", &letterWidth, &letterHeight);
    graphics->DrawText(L"S", x - w / 2 + SRFlipFlopLabelMargin, y - h / 2 + letterHeight / 2 + SRFlipFlopLabelMargin / 2);

    graphics->GetTextExtent(L"R", &letterWidth, &letterHeight);
    graphics->DrawText(L"R", x - w / 2 + SRFlipFlopLabelMargin, y + h / 2 - letterHeight - letterHeight / 2 - SRFlipFlopLabelMargin / 2);

    graphics->GetTextExtent(L"Q", &letterWidth, &letterHeight);
    graphics->DrawText(L"Q", x + w / 2 - SRFlipFlopLabelMargin - letterWidth, y - h / 2 + letterHeight / 2 + SRFlipFlopLabelMargin / 2);

    graphics->GetTextExtent(L"Q'", &letterWidth, &letterHeight);
    graphics->DrawText(L"Q'", x + w / 2 - SRFlipFlopLabelMargin - letterWidth, y + h / 2 - letterHeight - letterHeight / 2 - SRFlipFlopLabelMargin / 2);
}



/**
 * Sets an input if the wire end position is over an input pin.
 *
 * @param wirePos The wire end position in virtual pixels
 * @param wireOwner The Gate that wants to connect to an input
 */
void GateSRFlipFlop::OnReleaseWire(Vector wirePos, Pin *wireOwner)
{
    if (mInputPinS.HitTest(wirePos))
    {
        mInputPinS.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }

    else if (mInputPinR.HitTest(wirePos))
    {
        mInputPinR.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }
}

/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool GateSRFlipFlop::HitTest(Vector vec)
{
    // If output pin Q was clicked, start dragging a wire from it
    if (mOutputPinQ.HitTest(vec))
    {
        mOutputPinQ.SetWireEnd(vec);
        SetCurrentPin(&mOutputPinQ);
        return true;
    }

    // If output pin Q' was clicked, start dragging a wire from it
    if (mOutputPinQPrime.HitTest(vec))
    {
        mOutputPinQPrime.SetWireEnd(vec);
        SetCurrentPin(&mOutputPinQPrime);
        return true;
    }

    // Upcall to find out if clicked the object
    return Item::HitTest(vec);
}

/**
* Computes the state for the GateSRFlipFlop's output pins
*/
void GateSRFlipFlop::ComputeState()
{
    // Prevent cycles from crashing the program
    if (GetIsComputing())
    {
        mOutputPinQ.SetState(LogicState::Unknown);
        mOutputPinQPrime.SetState(LogicState::Unknown);
        return;
    }

    SetIsComputing(true);
    auto stateS = mInputPinS.ComputeState();
    auto stateR = mInputPinR.ComputeState();
    SetIsComputing(false);

    // If S=1 and R=1, this is forbidden: set both outputs to Unknown
    if (stateS == LogicState::One && stateR == LogicState::One)
    {
        mOutputPinQ.SetState(LogicState::Unknown);
        mOutputPinQPrime.SetState(LogicState::Unknown);
        return;
    }

    // If S=1, then Q=1 and Q'=0
    if (stateS == LogicState::One)
    {
        mOutputPinQ.SetState(LogicState::One);
        mOutputPinQPrime.SetState(LogicState::Zero);
        return;
    }

    // If R=1, or both S and R are unknown, then Q=0 and Q'=1
    if ((stateR == LogicState::One) || (stateS == LogicState::Unknown && stateR == LogicState::Unknown))
    {
        mOutputPinQ.SetState(LogicState::Zero);
        mOutputPinQPrime.SetState(LogicState::One);
    }

    // Otherwise, no change to output pins. They have memory, so we keep their current state as-is, even if both inputs are Zero
}


/**
 * Reset the Q and Q' outputs to their original values
 *
 * This gets called every time the conveyor starts
 */
void GateSRFlipFlop::Reset()
{
    mOutputPinQ.SetState(LogicState::Zero);
    mOutputPinQPrime.SetState(LogicState::One);
}