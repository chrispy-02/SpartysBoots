/**
 * @file Pin.cpp
 * @author Zaid Qourah
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include <iostream>

#include "Pin.h"

using namespace std;

/// Diameter to draw the pin in pixels
const int PinSize = 10;

/// Maximum offset of Bezier control points relative to wire ends
static constexpr double BezierMaxOffset = 200;

/**
 * Draws the pin and its incoming connection wire onto the graphics context
 * @param graphics The graphics context to draw to
 */
void Pin::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Set the color of the pin and wire
    switch (ComputeState())
    {
        case LogicState::One:
            mColor = ConnectionColorOne;
            break;
        case LogicState::Zero:
            mColor = ConnectionColorZero;
            break;
        case LogicState::Unknown:
            mColor = ConnectionColorUnknown;
            break;
    }

    wxPoint2DDouble pointStart(mPosition.GetX(), mPosition.GetY());
    wxPoint2DDouble pointEnd(mPosition.GetX() + mLength * (mType == PinType::Input ? -1 : 1), mPosition.GetY());

    // Set the pen to use pin color and wire thickness
    auto wirePen = graphics->CreatePen(wxGraphicsPenInfo(mColor, WireWidth)); // Pin color for Bézier curves
    graphics->SetPen(wirePen);

    // Disable the fill brush
    graphics->SetBrush(*wxTRANSPARENT_BRUSH);

    // If there is a wire connected to this Input pin,
    // or user is dragging a wire from this Output pin, draw the wire
    if (mIncomingPin != nullptr || mIsActive)
    {
        auto wirePos =
            mIncomingPin != nullptr ? (mIncomingPin->GetPosition() + Vector(mIncomingPin->GetLength(), 0)) : mWireEnd;
        auto offset = std::min(BezierMaxOffset, Vector(pointEnd.m_x, pointEnd.m_y).DistanceFrom(wirePos));

        if (mType == PinType::Output)
        {
            offset = -offset;
        }
        // Create the path for Bézier curve connection wire
        auto pathWire = graphics->CreatePath();
        pathWire.MoveToPoint(pointEnd);
        pathWire.AddCurveToPoint(pointEnd.m_x - offset, pointEnd.m_y, wirePos.GetX() + offset, wirePos.GetY(),
                                 wirePos.GetX(), wirePos.GetY());
        // Draw the Bézier curve connection wire
        graphics->DrawPath(pathWire);
    }

    // Draw the pin line
    graphics->StrokeLine(pointStart.m_x, pointStart.m_y, pointEnd.m_x, pointEnd.m_y);

    // Draw the pin circle with 1 pixel black outline and fill with mColor
    graphics->SetPen(*wxBLACK_PEN);
    graphics->SetBrush(wxBrush(mColor));
    graphics->DrawEllipse(pointEnd.m_x - PinSize / 2, pointEnd.m_y - PinSize / 2, PinSize, PinSize);
}

/**
 * Performs a hit test on the pin circle
 * @param vec The position
 * @return Whether the pin was hit
 */
bool Pin::HitTest(Vector vec)
{
    // multiply the length of the pin by -1 or 1, depending on Pin type
    auto offset = Vector(mLength * (mType == PinType::Input ? -1 : 1), 0);

    // Test if the pin circle has been hit
    return vec.DistanceFrom(mPosition + offset) <= (PinSize / 2);
}


/**
 * Computes the state of this Pin
 * @return The state of the Pin
 */
LogicState Pin::ComputeState()
{
    // If it is an Output pin, run ComputeState on the owner, which updates the pin state, then return pin state
    if (mType == PinType::Output)
    {
        mOwner->ComputeState();
        return mState;
    }

    // If this Input pin has a connection wire, return the ComputeState of the incoming pin
    if (mIncomingPin != nullptr)
    {
        return mIncomingPin->ComputeState();
    }

    // This is an Input pin with no connection, return state Unknown
    return LogicState::Unknown;
}
