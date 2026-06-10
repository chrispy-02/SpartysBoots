/**
 * @file PinOwner.h
 * @author Elijah Porter
 *
 * The interface of devices that can own a Pin.
 */

#pragma once

#include "LogicState.h"
#include "Vector.h"

class Pin;
class Game;

/**
 * The PinOwner interface
 */
class PinOwner
{
  private:
  public:
    virtual ~PinOwner() = default;

    /// Computes the state for each of the PinOwner's output pins
    virtual void ComputeState() = 0;


    /**
     * Sets the current pin being dragged
     * @param mousePos The mouse position in virtual pixels
     */
    virtual void Dragged(Vector mousePos) {}

    /**
     * Detects which pin is released over and connects the line
     * @param vec The mouse position in virtual pixels
     */
    virtual void Released(Vector vec) {}
};
