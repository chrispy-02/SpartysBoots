/**
 * @file Product.cpp
 * @author Zaid Qourah
 * @author Bob Wilson
 * @author Elijah Porter
 */

#include "pch.h"

#include <iostream>

#include "Product.h"

using namespace std;

/// Size to draw content relative to the product Size
const double ContentScale = 0.8;

/// Default product Size in pixels
const double Product::Size = 80;

/**
 * Constructor
 * @param game Game this Conveyor is a member of
 */
Product::Product(Game *game) : Item(game, L"") {}

/**
 * Get the list of properties for this product.
 * @return A vector of product properties
 */
const std::vector<Product::Properties> &Product::GetProperties() const { return mProperties; }

/**
 * Update the product's movement on the conveyor.
 * @param elapsed Time passed since the last update
 */
void Product::Update(double elapsed)
{
    // If the product has been kicked, it moves to the left
    if (mKicked)
    {
        SetX(GetX() - GetGame()->GetKickSpeed() * elapsed);
        return;
    }

    // If it has not been kicked and conveyor is moving, it moves down
    if (GetGame()->IsConveyorMoving())
    {
        SetY(GetY() + GetGame()->GetConveyorSpeed() * elapsed);
    }

    // Check if the product has passed the beam
    if (!mPassedBeam && GetY() > GetGame()->GetBeamY())
    {
        mPassedBeam = true;
    }
}


const std::map<std::wstring, Product::Properties> Product::NamesToProperties = {
    {L"red", Product::Properties::Red},           {L"green", Product::Properties::Green},
    {L"blue", Product::Properties::Blue},         {L"white", Product::Properties::White},
    {L"square", Product::Properties::Square},     {L"circle", Product::Properties::Circle},
    {L"diamond", Product::Properties::Diamond},   {L"izzo", Product::Properties::Izzo},
    {L"smith", Product::Properties::Smith},       {L"basketball", Product::Properties::Basketball},
    {L"football", Product::Properties::Football}, {L"none", Product::Properties::None},
};

const std::map<Product::Properties, Product::Types> Product::PropertiesToTypes = {
    {Product::Properties::Red, Product::Types::Color},
    {Product::Properties::Green, Product::Types::Color},
    {Product::Properties::Blue, Product::Types::Color},
    {Product::Properties::White, Product::Types::Color},
    {Product::Properties::Square, Product::Types::Shape},
    {Product::Properties::Circle, Product::Types::Shape},
    {Product::Properties::Diamond, Product::Types::Shape},
    {Product::Properties::Izzo, Product::Types::Content},
    {Product::Properties::Smith, Product::Types::Content},
    {Product::Properties::Football, Product::Types::Content},
    {Product::Properties::Basketball, Product::Types::Content},
    {Product::Properties::None, Product::Types::Content}};

// Maps content properties to their associated images
const std::map<Product::Properties, std::wstring> Product::PropertiesToContentImages = {
    {Product::Properties::Izzo, L"izzo.png"},
    {Product::Properties::Smith, L"smith.png"},
    {Product::Properties::Football, L"football.png"},
    {Product::Properties::Basketball, L"basketball.png"}};


/**
 * Draw the product on the screen.
 * @param graphics The wxGraphicsContext to draw on
 */
void Product::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{

    // remove any pen so it doesn't draw an outline around the shape
    graphics->SetPen(wxNullPen);

    // Set the product color and fill the shape
    wxBrush brush;
    if (std::find(mProperties.begin(), mProperties.end(), Properties::Red) != mProperties.end())
    {
        brush.SetColour(ColorRed);
    }
    else if (std::find(mProperties.begin(), mProperties.end(), Properties::Green) != mProperties.end())
    {
        brush.SetColour(ColorGreen);
    }
    else if (std::find(mProperties.begin(), mProperties.end(), Properties::Blue) != mProperties.end())
    {
        brush.SetColour(ColorBlue);
    }
    else if (std::find(mProperties.begin(), mProperties.end(), Properties::White) != mProperties.end())
    {
        brush.SetColour(*wxWHITE);
    }
    graphics->SetBrush(brush);

    // Draw the product shape (square, circle, or diamond)
    if (std::find(mProperties.begin(), mProperties.end(), Properties::Square) != mProperties.end())
    {
        graphics->DrawRectangle(GetX() - Size / 2, GetY() - Size / 2, Size, Size);
    }
    else if (std::find(mProperties.begin(), mProperties.end(), Properties::Circle) != mProperties.end())
    {
        graphics->DrawEllipse(GetX() - Size / 2, GetY() - Size / 2, Size, Size);
    }
    else if (std::find(mProperties.begin(), mProperties.end(), Properties::Diamond) != mProperties.end())
    {
        graphics->PushState();
        graphics->Translate(GetX(), GetY());
        graphics->Rotate(M_PI / 4); // 45 degrees
        graphics->DrawRectangle(-Size / 2, -Size / 2, Size, Size);
        graphics->PopState();
    }

    // Draw content images if applicable
    for (const auto &property : mProperties)
    {
        if (PropertiesToContentImages.find(property) != PropertiesToContentImages.end())
        {
            std::wstring imageFile = PropertiesToContentImages.at(property);

            // Use the custom Image class for loading and drawing the image
            auto contentImage = GetGame()->GetAssetManager().GetImage(imageFile, graphics);

            double contentSize = Size * ContentScale;

            // Draw the image on top of the product shape, scaled to fit the product Size
            graphics->DrawBitmap(contentImage->GetBitmap(), GetX() - contentSize / 2, GetY() - contentSize / 2,
                                 contentSize, contentSize);
        }
    }
}


/**
 * Load the attributes for a Product node.
 *
 * This overrides the function in case class Item
 *
 * @param node The Xml node we are loading the product from
 * @param conveyorPosition
 */
void Product::XmlLoad(wxXmlNode *node, Vector conveyorPosition)
{
    static double previousPlacement = 0.0; // to help with relative placement for Product | ZQ

    // Extract the properties from the XML node
    std::wstring placementString = node->GetAttribute(L"placement").ToStdWstring();
    std::wstring shapeName = node->GetAttribute(L"shape").ToStdWstring();
    std::wstring colorName = node->GetAttribute(L"color").ToStdWstring();
    std::wstring contentName = node->GetAttribute(L"content").ToStdWstring();
    mSupposedToBeKicked = (node->GetAttribute(L"kick") == L"yes");

    // Handle relative and absolute placements
    double placement;
    if (placementString[0] == L'+')
    {
        // Handle relative placement (e.g., "+150")
        placement = previousPlacement + std::stod(placementString.substr(1));
    }
    else
    {
        // Handle absolute placement and reset the previous placement
        placement = std::stod(placementString);
        previousPlacement = placement;
    }

    // Update the previous placement for the next product (ensure it's done only after absolute placement)
    previousPlacement = placement;

    SetPosition(Vector(conveyorPosition - Vector(0, placement)));

    // set the initial position, the product will get reset to this every time the conveyor starts
    mInitialPosition = GetPosition();

    // Find the corresponding properties from the XML attributes
    if (Product::NamesToProperties.find(shapeName) != Product::NamesToProperties.end())
    {
        mProperties.push_back(Product::NamesToProperties.at(shapeName));
    }
    if (Product::NamesToProperties.find(colorName) != Product::NamesToProperties.end())
    {
        mProperties.push_back(Product::NamesToProperties.at(colorName));
    }
    if (!contentName.empty() && Product::NamesToProperties.find(contentName) != Product::NamesToProperties.end())
    {
        mProperties.push_back(Product::NamesToProperties.at(contentName));
    }
}

/**
 * Reset position to initial starting position, and set mKicked, mPassedBeam, and mHasBeenScored to false
 *
 * This gets called every time the conveyor starts
 */
void Product::Reset()
{
    SetPosition(mInitialPosition);
    mKicked = false;
    mPassedBeam = false;
    mHasBeenScored = false;
}

/**
 * Test Product to see if crossing conveyor
 *
 * @param beamY The Y value of beam in virtual pixels
 * @return A boolean of whether the product is crossing the beam
 */
bool Product::IsCrossingBeam(double beamY)
{
    // If the product has been kicked, it cannot be crossing the beam
    if (mKicked)
    {
        return false;
    }

    bool crossing = (beamY < GetY() + Size / 2) && (beamY > GetY() - Size / 2);

    // Kick the product if told to
    if (crossing && GetGame()->ShouldKickProduct())
    {
        mKicked = true;
        GetGame()->KickProduct(false);
    }

    return crossing;
}

/**
 * Tells whether the product has been processed (kicked or passed beam)
 *
 * @return Whether the product has been processed
 */
bool Product::IsProcessed() const { return mKicked || mPassedBeam; }
