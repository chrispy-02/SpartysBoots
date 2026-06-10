/**
 * @file GateDFlipFlop.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include <iostream>
#include <math.h>
#include "GateDFlipFlop.h"
#include "Pin.h"

using namespace std;

/// Size of the Flip Flop in pixels
/// @return None
const wxSize DFlipFlopSize(50, 75);

/// Gap between the edge of the flip flop and the labels
/// @return None
const int DFlipFlopLabelMargin = 3;

/// How large the clock input triagle is in pixels width and height
/// @return None
const int DFlipFlopClockSize = 10;

/// The font size for the Gate
const int FontSize = 15;


/**
 * The D Flip Flop Gate constructor.
 *
 * @param game The game this Gate belongs to
 */
GateDFlipFlop::GateDFlipFlop(Game *game)
    : Gate(game, DFlipFlopSize.GetWidth(), DFlipFlopSize.GetHeight())
{
    mOutputPinQ.SetState(LogicState::Zero);
    mOutputPinQPrime.SetState(LogicState::One);
}


/**
 * Updates the positions of the children pins
 */
void GateDFlipFlop::UpdatePinPositions()
{
    auto width = GetWidth();
    auto halfHeight = GetHeight() / 2;

    // Set the position of the input pins (D and Clock)
    mInputPinD.SetPosition(GetPosition() - Vector(width / 2, halfHeight / 2));  // D input (Top left)
    mInputPinClock.SetPosition(GetPosition() - Vector(width / 2, -halfHeight / 2)); // Clock input (Bottom left)

    // Set the position of the output pins (Q and Q')
    mOutputPinQ.SetPosition(GetPosition() + Vector(width / 2, -halfHeight / 2));  // Q output (Top right)
    mOutputPinQPrime.SetPosition(GetPosition() + Vector(width / 2, halfHeight / 2));  // Q' output (Bottom right)
}

/**
 * Draws the GateDFlipFlop on a given Graphics Context
 *
 * @param graphics The Graphics Context to draw on
 */
void GateDFlipFlop::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Update pin positions
    UpdatePinPositions();

    // Draw the pins
    mInputPinD.Draw(graphics);  // Draw D input pin
    mInputPinClock.Draw(graphics);  // Draw Clock input pin

    // Draw Q output pin (it should be black if there are no input connections)
    mOutputPinQ.Draw(graphics);

    // Draw Q' output pin (it should be red if there are no input connections)
    mOutputPinQPrime.Draw(graphics);


    // The location and size
    auto x = GetX();
    auto y = GetY();
    auto w = GetWidth();
    auto h = GetHeight();

    // Draw the path
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(*wxWHITE_BRUSH);
    graphics->DrawRectangle(x - w/2, y - h/2, w, h);


    auto font = graphics->CreateFont(FontSize, L"Arial",
                             wxFONTFLAG_BOLD, *wxBLACK);
    // Draw the letters
    double letterWidth, letterHeight;
    graphics->GetTextExtent(L"D", &letterWidth, &letterHeight);
    graphics->DrawText(L"D", x - w/2 + DFlipFlopLabelMargin,
                             y - h/2 + letterHeight/2 + DFlipFlopLabelMargin/2);

    graphics->GetTextExtent(L"Q", &letterWidth, &letterHeight);
    graphics->DrawText(L"Q", x + w/2 - DFlipFlopLabelMargin - letterWidth,
                             y - h/2 + letterHeight/2 + DFlipFlopLabelMargin/2);

    graphics->GetTextExtent(L"Q'", &letterWidth, &letterHeight);
    graphics->DrawText(L"Q'", x + w/2 - DFlipFlopLabelMargin - letterWidth,
                              y + h/2 - letterHeight - letterHeight/2 - DFlipFlopLabelMargin/2);


    // The three corner points of a D Flip Flop Gate gate
    wxPoint2DDouble bl(x - w/2, y + h/2 - letterHeight/2 - DFlipFlopClockSize/2); // Bottom left

    wxPoint2DDouble rm(bl.m_x + DFlipFlopClockSize, bl.m_y - DFlipFlopClockSize/2); // Right middle

    wxPoint2DDouble tl(bl.m_x, bl.m_y - DFlipFlopClockSize); // Top left

    // Create a path to draw the gate shape
    auto path = graphics->CreatePath();

    // Create the path for the gate
    path.MoveToPoint(tl);
    path.AddLineToPoint(rm);
    path.AddLineToPoint(bl);
    path.CloseSubpath();

    graphics->DrawPath(path);
}

/**
 * Sets an input if the wire end position is over an input pin.
 *
 * @param wirePos The wire end position in virtual pixels
 * @param wireOwner The Gate that wants to connect to an input
 */
void GateDFlipFlop::OnReleaseWire(Vector wirePos, Pin *wireOwner)
{
    if (mInputPinD.HitTest(wirePos))
    {
        mInputPinD.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }

    else if (mInputPinClock.HitTest(wirePos))
    {
        mInputPinClock.SetIncomingPin(wireOwner);
        SetCurrentPin(nullptr);
    }
}

/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool GateDFlipFlop::HitTest(Vector vec)
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
 * Computes the state for the GateDFlipFlop's output pins
 */
void GateDFlipFlop::ComputeState()
{
    // Prevent cycles from crashing the program
    if (GetIsComputing())
    {
        mOutputPinQ.SetState(LogicState::Unknown);
        mOutputPinQPrime.SetState(LogicState::Unknown);
        return;
    }

    auto previousStateClock = mInputPinClock.GetState();

    SetIsComputing(true);
    auto stateD = mInputPinD.ComputeState();
    auto stateClock = mInputPinClock.ComputeState();
    SetIsComputing(false);

    // Set the state of PinClock to keep track of it, to know when it changes from 0 to 1
    mInputPinClock.SetState(stateClock);

    // if both input states are unknown, Q=0 and Q'=1
    if (stateD == LogicState::Unknown && stateClock == LogicState::Unknown)
    {
        mOutputPinQ.SetState(LogicState::Zero);
        mOutputPinQPrime.SetState(LogicState::One);
        return;
    }

    // When Clock is switched from 0 to 1, Q=D and Q'=!D
    if (previousStateClock == LogicState::Zero && stateClock == LogicState::One)
    {
        if (stateD == LogicState::One)
        {
            mOutputPinQ.SetState(LogicState::One);
            mOutputPinQPrime.SetState(LogicState::Zero);
            return;
        }
        mOutputPinQ.SetState(LogicState::Zero);
        mOutputPinQPrime.SetState(LogicState::One);
    }
}


/**
 * Reset the Q and Q' outputs to their original values
 *
 * This gets called every time the conveyor starts
 */
void GateDFlipFlop::Reset()
{
    mOutputPinQ.SetState(LogicState::Zero);
    mOutputPinQPrime.SetState(LogicState::One);
    mInputPinClock.SetState(LogicState::Unknown);
}