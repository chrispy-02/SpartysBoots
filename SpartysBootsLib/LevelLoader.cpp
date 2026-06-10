/**
* @file LevelLoader.cpp
*
* @author Bob Wilson
*/

#include "pch.h"
#include "LevelLoader.h"
#include <iostream>
#include "Game.h"
#include "Scoreboard.h"
#include "Conveyor.h"
#include "ConveyorBackground.h"
#include "ConveyorPanel.h"
#include "Sparty.h"
#include "Beam.h"
#include "Sensor.h"
#include "Product.h"

using namespace std;


/**
* LevelLoader constructor
*
* @param game The game
*/
LevelLoader::LevelLoader(Game* game) : mGame(game)
{
}


/**
* Load the level from a .xml file.
*
* Opens the XML file and reads the nodes, creating items as appropriate.
*
* @param filename The filename of the file to load the game from.
*/
void LevelLoader::Load(const wxString &filename)
{
    // cout << endl << "LevelLoader::Load on " << filename << endl;
    wxXmlDocument xmlDoc;
    if(!xmlDoc.Load(filename))
    {
        wxMessageBox(L"Unable to load level file: " + filename);
        return;
    }

    mGame->Clear();

    // Get the XML document root node
    auto root = xmlDoc.GetRoot(); // this should be the tag, for example <level size="1150,800">

    // load width and height
    string size = root->GetAttribute(L"size").ToStdString();
    size_t pos = size.find(',');


    // set the game dimensions
    mGame->SetGameDimensions(stoi(size.substr(0, pos)),stoi(size.substr(pos + 1)));

    auto items = root->GetChildren();

    // Traverse the children of the items node of the XML document in memory!!!!
    auto child = items->GetChildren();
    for( ; child; child=child->GetNext())
    {
        auto name = child->GetName();
        // cout << "LevelLoader::Load is loading a " << name << endl;
        if(name == L"conveyor")
        {
            XmlItem(child); // create the ConveyorBackground and 2 conveyors (one on-screen and one above offscreen)
        }
        else if(name == L"sensor")
        {
            XmlItem(child);
        }
        else if(name == L"beam")
        {
            XmlItem(child);
        }
        else if(name == L"sparty")
        {
            XmlItem(child);
        }
        else if(name == L"scoreboard")
        {
            XmlItem(child);
        }
        else
        {
            cout << "LevelLoader::Load found unexpected tag " << name << " in the XML file!" << endl;
        }
    }

    // Traverse the children of the items node of the XML document in memory again for Products.
    // Products need to be drawn on top of everything else, so we load them last.
    child = items->GetChildren();
    for( ; child; child=child->GetNext())
    {
        auto name = child->GetName();
        if(name == L"conveyor")
        {
            Vector conveyorPosition(stod(child->GetAttribute(L"x").ToStdString()),
                                    stod(child->GetAttribute(L"y").ToStdString()));

            auto productNode = child->GetChildren();
            shared_ptr<Product> product = nullptr;

            for( ; productNode; productNode = productNode->GetNext())
            {
                // cout << "LevelLoader::Load is loading a " <<  productNode->GetName() << endl;

                product = make_shared<Product>(mGame);
                product->XmlLoad(productNode, conveyorPosition);

                mGame->Add(product);
            }

            // Let the last product know that it is the last product
            if (product != nullptr)
            {
                product->SetIsLast(true);
            }
        }
    }

    // After loading all items, reset game state
    mGame->ResetLevelScore();
}


/**
* Handle a node of type item.
* @param node XML node
*/
void LevelLoader::XmlItem(wxXmlNode *node)
{
    // We have an item. What kind?
    auto name = node->GetName();
    // cout << "\tLevelLoader::XmlItem on wxXmlNode " << name << endl;

    if (name == L"conveyor")
    {
        // Create the ConveyorBackground
        shared_ptr<ConveyorBackground> conveyorBackground = make_shared<ConveyorBackground>(mGame);
        mGame->Add(conveyorBackground);
        conveyorBackground->XmlLoad(node);

        // Create the onscreen Conveyor
        shared_ptr<Conveyor> conveyorOnscreen = make_shared<Conveyor>(mGame);
        mGame->Add(conveyorOnscreen);
        conveyorOnscreen->XmlLoad(node);

        // Create the offscreen Conveyor (above the top of the screen)
        shared_ptr<Conveyor> conveyorOffscreen = make_shared<Conveyor>(mGame);
        mGame->Add(conveyorOffscreen);
        conveyorOffscreen->XmlLoad(node);
        conveyorOffscreen->SetPosition(conveyorOffscreen->GetPosition() - Vector(0, mGame->GetHeight()));

        // Create a ConveyorPanel
        shared_ptr<ConveyorPanel> conveyorPanel = make_shared<ConveyorPanel>(mGame);
        mGame->Add(conveyorPanel);
        // the ConveyorPanel position is relative to the Conveyor, so we pass the Conveyor position as a parameter
        conveyorPanel->XmlLoad(node, conveyorOnscreen->GetPosition());
    }

    else if (name == L"sensor")
    {
        shared_ptr<Sensor> sensor = make_shared<Sensor>(mGame);
        mGame->Add(sensor);
        sensor->XmlLoad(node);

    }

    else if (name == L"beam")
    {
        shared_ptr<Beam> beam = make_shared<Beam>(mGame);
        mGame->Add(beam);
        beam->XmlLoad(node);
    }
    else if (name == L"sparty")
    {
        shared_ptr<Sparty> item1 = make_shared<Sparty>(mGame);
        mGame->Add(item1);
        item1->XmlLoad(node);
    }
    else if (name == L"scoreboard")
    {
        shared_ptr<Scoreboard> item1 = make_shared<Scoreboard>(mGame);
        mGame->Add(item1);
        item1->XmlLoad(node);
    }

}
