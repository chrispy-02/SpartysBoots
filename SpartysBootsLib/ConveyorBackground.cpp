/**
* @file ConveyorBackground.cpp
*
* @author Bob Wilson
*/

#include "pch.h"

#include "ConveyorBackground.h"
#include <string>

using namespace std;


/// Image for the background (base) of the conveyor
const std::wstring ConveyorBackgroundImage = L"conveyor-back.png";


/**
* Constructor
* @param game Game this ConveyorBackground is a member of
*/
ConveyorBackground::ConveyorBackground(Game *game) : Item(game, ConveyorBackgroundImage)
{
}


/**
* Load the attributes for a ConveyorBackground node.
*
* This overrides the function in case class Item
*
* @param node The Xml node we are loading the item from
*/
void ConveyorBackground::XmlLoad(wxXmlNode *node)
{
    GetGame()->SetConveyorSpeed(stod(node->GetAttribute(L"speed").ToStdString()));

    double x = stod(node->GetAttribute(L"x").ToStdString());
    double y = stod(node->GetAttribute(L"y").ToStdString());
    double height = stod(node->GetAttribute(L"height").ToStdString());

    SetPosition(Vector(x, y));

    // scale the conveyor background (maintaining aspect ratio) to fit the height of the level
    SetTargetDimensions(GetWidth() * height / GetHeight(), height);
}
