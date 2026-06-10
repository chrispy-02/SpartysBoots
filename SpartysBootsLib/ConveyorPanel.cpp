/**
 * @file ConveyorPanel.cpp
 *
 * @author Bob Wilson
 * @author Elijah Porter
 */

#include "pch.h"

#include <string>
#include "ConveyorPanel.h"

using namespace std;


/// Image for the conveyor control panel when stopped.
const std::wstring ConveyorPanelStoppedImage = L"conveyor-switch-stop.png";

/// Image for the conveyor control panel when started.
const std::wstring ConveyorPanelStartedImage = L"conveyor-switch-start.png";

/**
 * Rectangle representing the location of the start button relative to the panel
 *
 * This is not really a function, it is a constant wxRect. But we were getting Doxygen errors,
 * so Jasen Van Acker told me on Interact to format it like this and it fixes the Doxygen errors.
 * @return: wxRect object
 */
const wxRect StartButtonRect(35, 29, 95, 36);

/**
 * Rectangle representing the location of the stop button relative to the panel
 *
 * This is not really a function, it is a constant wxRect. But we were getting Doxygen errors,
 * so Jasen Van Acker told me on Interact to format it like this and it fixes the Doxygen errors.
 * @return: wxRect object
 */
const wxRect StopButtonRect(35, 87, 95, 36);


/**
 * Constructor
 * @param game Game this ConveyorPanel is a member of
 */
ConveyorPanel::ConveyorPanel(Game *game) : Item(game, ConveyorPanelStoppedImage) {}


/**
 * Load the attributes for a ConveyorPanel node.
 *
 * This overrides the function in case class Item
 *
 * @param node The Xml node we are loading the item from
 * @param conveyorPosition The position of the conveyor (the panel position is relative to it)
 */
void ConveyorPanel::XmlLoad(wxXmlNode *node, Vector conveyorPosition)
{
    // load width and height
    string panelPosition = node->GetAttribute(L"panel").ToStdString();
    size_t pos = panelPosition.find(',');
    double x = stoi(panelPosition.substr(0, pos));
    double y = stoi(panelPosition.substr(pos + 1));
    SetPosition(Vector(x, y) + Vector(GetWidth() / 2, GetHeight() / 2) + conveyorPosition);
}


/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The hit position in virtual pixels
 * @return Whether the mouse clicked this object
 */
bool ConveyorPanel::HitTest(Vector vec)
{
    // do an upcall to find out if the ConveyorPanel was clicked
    if (Item::HitTest(vec))
    {
        // was the Start button clicked?
        if (vec.GetX() >= (GetX() - GetWidth() / 2 + StartButtonRect.GetX()) &&
            vec.GetX() <= (GetX() - GetWidth() / 2 + StartButtonRect.GetX() + StartButtonRect.GetWidth()) &&
            vec.GetY() >= (GetY() - GetHeight() / 2 + StartButtonRect.GetY()) &&
            vec.GetY() <= (GetY() - GetHeight() / 2 + StartButtonRect.GetY() + StartButtonRect.GetHeight()))
        {
            GetGame()->ConveyorStart();
        }

        // was the Stop button clicked?
        else if (vec.GetX() >= (GetX() - GetWidth() / 2 + StopButtonRect.GetX()) &&
                 vec.GetX() <= (GetX() - GetWidth() / 2 + StopButtonRect.GetX() + StopButtonRect.GetWidth()) &&
                 vec.GetY() >= (GetY() - GetHeight() / 2 + StopButtonRect.GetY()) &&
                 vec.GetY() <= (GetY() - GetHeight() / 2 + StopButtonRect.GetY() + StopButtonRect.GetHeight()))
        {
            GetGame()->ConveyorStop();
        }
        return true;
    }
    return false;
}


/**
 * Draws the ConveyorPanel to a given GC.
 *
 * @param graphics The GC to draw on
 */
void ConveyorPanel::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    auto imageStopped = GetGame()->GetAssetManager().GetImage(ConveyorPanelStoppedImage, graphics);
    auto imageStarted = GetGame()->GetAssetManager().GetImage(ConveyorPanelStartedImage, graphics);

    double wid = GetWidth();
    double hit = GetHeight();

    if (GetGame()->IsConveyorMoving())
    {
        graphics->DrawBitmap(imageStarted->GetBitmap(), GetX() - wid / 2, GetY() - hit / 2, wid, hit);
    }
    else
    {
        graphics->DrawBitmap(imageStopped->GetBitmap(), GetX() - wid / 2, GetY() - hit / 2, wid, hit);
    }
}
