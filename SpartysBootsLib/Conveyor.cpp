/**
 * @file Conveyor.cpp
 *
 * @author Bob Wilson
 */

#include "pch.h"

#include <string>

#include "Conveyor.h"

using namespace std;

/// Image for the conveyor belt
const std::wstring ConveyorBeltImage = L"conveyor-belt.png";

/// The point at which the conveyor is considered off the screen
const double ConveyorGoneMultiple = 1.5;

/**
 * Constructor
 * @param game Game this Conveyor is a member of
 */
Conveyor::Conveyor(Game *game) : Item(game, ConveyorBeltImage) {}


/**
 * Load the attributes for a Conveyor node.
 *
 * This overrides the function in case class Item
 *
 * @param node The Xml node we are loading the item from
 */
void Conveyor::XmlLoad(wxXmlNode *node)
{
    double x = stod(node->GetAttribute(L"x").ToStdString());
    double y = stod(node->GetAttribute(L"y").ToStdString());
    double height = stod(node->GetAttribute(L"height").ToStdString());

    // Set the position of the conveyor
    SetPosition(Vector(x, y));

    // scale the conveyor (maintaining aspect ratio) to fit the height of the level
    SetTargetDimensions(GetWidth() * height / GetHeight(), height);
}


/**
 * Handle updates in time of our conveyor
 *
 * This is called before we draw and allows us to
 * move our conveyor. We add our speed times the amount
 * of time that has elapsed.
 * @param elapsed Time elapsed since the class call
 */
void Conveyor::Update(double elapsed)
{
    if (GetGame()->IsConveyorMoving())
    {
        double y = GetY() + GetGame()->GetConveyorSpeed() * elapsed;

        // If it has gone off the bottom of the screen, move it to the area above the top of the screen
        if (y > GetGame()->GetHeight() * ConveyorGoneMultiple)
        {
            y -= GetGame()->GetHeight() * 2;
        }

        SetY(y);
    }
}
