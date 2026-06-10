/**
 * @file Gate.cpp
 * @author Elijah Porter
 * @author Bob Wilson
 */

#include "pch.h"

#include "Game.h"
#include "Gate.h"
#include "Pin.h"
#include <algorithm>

/**
 * Gate constructor
 *
 * @param game The Game this Gate belongs to
 * @param wid The width of this Gate
 * @param hit The height of this Gate
 */
Gate::Gate(Game *game, double wid, double hit) : Item(game, L"")
{
    mWidth = wid;
    mHeight = hit;
}

/**
 * Moves the Gate to the mouse position when being dragged.
 *
 * If the user is dragging a wire, set the wire endpoint to the mouse position.
 *
 * @param mousePos The mouse position in virtual pixels
 */
void Gate::Dragged(Vector mousePos)
{
    auto currentPin = GetCurrentPin();

    // If dragging a wire, update the position of the end of the wire
    if (currentPin != nullptr)
    {
        currentPin->SetWireEnd(mousePos);
    }

    // Otherwise it is dragging the Gate around, update its position and gate positions
    else
    {
        // Set position, but make sure it doesn't go off screen.
        SetPosition(Vector(std::min(std::max(mousePos.GetX(), GetWidth()), GetGame()->GetWidth() - GetWidth()),
                           std::min(std::max(mousePos.GetY(),  GetHeight() / 2), GetGame()->GetHeight() - GetHeight()/ 2)));
        UpdatePinPositions();
    }
}

/**
 * Sets the current pin being dragged
 *
 * @param pin the pin currently being dragged
 */
void Gate::SetCurrentPin(Pin *pin)
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
 * If the wire is released over an input pin, connect the pins
 *
 * @param mousePos The mouse position in virtual pixels
 */
void Gate::Released(Vector mousePos)
{
    auto currentPin = GetCurrentPin();

    if (currentPin != nullptr)
    {
        OnReleaseWire(mousePos, currentPin);
    }
}



