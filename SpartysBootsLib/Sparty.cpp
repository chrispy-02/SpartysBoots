/**
 * @file Sparty.cpp
 * @author Christopher Nguyen
 *
 * Implementation of the Sparty class.
 */

#include "pch.h"

#include "Game.h"
#include "Pin.h"
#include "Sparty.h"

using namespace std;

/// Image for the Sparty background (behind the boot)
const std::wstring SpartyBackImage = L"sparty-back.png";

/// Image for the Sparty boot (the leg that rotates)
const std::wstring SpartyBootImage = L"sparty-boot.png";

/// Image for the Sparty foreground (in front of the boot)
const std::wstring SpartyFrontImage = L"sparty-front.png";

/// The vertical offset of Sparty's wire
const int WireHorizontalOffset = 80;

/// The vertical offset of Sparty's wire
const int WireVerticalOffset = 20;

/// The point in the kick animation when the product actually is kicked
const double SpartyKickPoint = 0.35;

/**
 * Constructor.
 * @param game The game this item belongs to
 */
Sparty::Sparty(Game *game) : Item(game, L"") // No main image for Sparty
{
    // Load the images using the AssetManager
    mBackImage = game->GetAssetManager().GetImage(SpartyBackImage);
    mBootImage = game->GetAssetManager().GetImage(SpartyBootImage);
    mFrontImage = game->GetAssetManager().GetImage(SpartyFrontImage);
}

/**
 * Load the attributes for an item node.
 *
 * @param node The Xml node we are loading the item from
 */
void Sparty::XmlLoad(wxXmlNode *node)
{
    // Load x and y attributes
    double x = stod(node->GetAttribute(L"x", L"0").ToStdWstring());
    double y = stod(node->GetAttribute(L"y", L"0").ToStdWstring());
    SetPosition(Vector(x, y));

    // Load the height and calculate the width to maintain aspect ratio
    double height = stod(node->GetAttribute(L"height", L"300").ToStdWstring());
    double backImageHeight = mBackImage->GetHeight();
    double backImageWidth = mBackImage->GetWidth();
    double width = (backImageWidth / backImageHeight) * height;

    // Store the calculated dimensions
    mWidth = width;
    mHeight = height;

    // Load the input pin position
    std::wstring pinStr = node->GetAttribute(L"pin", L"0,0").ToStdWstring();
    size_t commaPos = pinStr.find(L',');
    double pinX = stod(pinStr.substr(0, commaPos));
    double pinY = stod(pinStr.substr(commaPos + 1));

    mInputPin.SetPosition(Vector(pinX + mInputPin.GetLength(), pinY));

    // Load kick-duration and kick-speed
    mKickDuration = stod(node->GetAttribute(L"kick-duration", L"0.25").ToStdWstring());
    GetGame()->SetKickSpeed(stod(node->GetAttribute(L"kick-speed").ToStdWstring()));
}

/**
 * The progress of Sparty's kick
 * @return The kick progress
 */
double Sparty::GetKickProgress() { return fmod(mKickTime, mKickDuration) / mKickDuration; }

/**
 * Draw Sparty
 * @param graphics Graphics context to draw on
 */
void Sparty::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Ensure images have their bitmaps created
    mBackImage->CreateBitmap(graphics);
    mBootImage->CreateBitmap(graphics);
    mFrontImage->CreateBitmap(graphics);

    double wid = mWidth;
    double hit = mHeight;

    double x = GetX() - wid / 2;
    double y = GetY() - hit / 2;

    mInputPin.Draw(graphics);

    auto color = mInputPin.GetColor();

    // Set the pen to use pin color and wire thickness
    auto wirePen = graphics->CreatePen(wxGraphicsPenInfo(color, WireWidth)); // Pin color for Bézier curves
    graphics->SetPen(wirePen);

    // Disable the fill brush
    graphics->SetBrush(*wxTRANSPARENT_BRUSH);

    // Draw the wire
    graphics->StrokeLine(mInputPin.GetPosition().GetX(), mInputPin.GetPosition().GetY(), mInputPin.GetPosition().GetX(),
                         WireVerticalOffset);
    graphics->StrokeLine(GetPosition().GetX() + WireHorizontalOffset, WireVerticalOffset,
                         mInputPin.GetPosition().GetX(), WireVerticalOffset);
    graphics->StrokeLine(GetPosition().GetX() + WireHorizontalOffset, WireVerticalOffset,
                         GetPosition().GetX() + WireHorizontalOffset, GetPosition().GetY());
    graphics->StrokeLine(GetPosition().GetX(), GetPosition().GetY(), GetPosition().GetX() + WireHorizontalOffset,
                         GetPosition().GetY());

    // Draw the back image
    graphics->DrawBitmap(mBackImage->GetBitmap(), x, y, wid, hit);


    // Draw the boot image with rotation
    graphics->PushState(); // Save the current state

    // Calculate the rotation angle based on kick animation
    double angle = 0.0;

    // Calculate the percentage of the kick completed
    double kickProgress = GetKickProgress();

    if (kickProgress < 0.5)
    {
        // Rotating backward
        angle = MaxBootRotation * (kickProgress / 0.5);
    }
    else
    {
        // Rotating back to original position
        angle = MaxBootRotation * ((1.0 - kickProgress) / 0.5);
    }

    // Set the rotation
    // Calculate the pivot point in absolute coordinates
    double pivotX = GetX() - wid / 2 + wid * mBootPivot.m_x;
    double pivotY = GetY() - hit / 2 + hit * mBootPivot.m_y;

    graphics->Translate(pivotX, pivotY);
    graphics->Rotate(angle);
    graphics->Translate(-pivotX, -pivotY);

    // Draw the boot image
    graphics->DrawBitmap(mBootImage->GetBitmap(), x, y, wid, hit);

    graphics->PopState(); // Restore the state after drawing

    // Draw the front image
    graphics->DrawBitmap(mFrontImage->GetBitmap(), x, y, wid, hit);

    // Optional: Draw a line where the boot tip is (for debugging)
    /*
    int bootY = int(hit * BootTipPercentage);
    wxPen bootablePen(*wxGREEN, 3);
    graphics->SetPen(bootablePen);
    graphics->StrokeLine(GetX() - wid / 2, GetY() - hit / 2 + bootY,
                         GetX() + wid / 2, GetY() - hit / 2 + bootY);
    */
}

/**
 * Update Sparty's state
 * @param elapsed Time elapsed since last update
 */
void Sparty::Update(double elapsed)
{
    auto currentState = mInputPin.ComputeState();

    if (mIsKicking)
    {
        mKickTime += elapsed;

        // The SpartyKickPoint is for when it is 35% through the first part of the kick, so this must be divided by two
        auto kickPoint = mKickDuration * SpartyKickPoint * 0.5;

        // Kick the product if the boot is rotated enough
        if (mKickTime >= kickPoint && mKickTime - elapsed < kickPoint)
        {
            GetGame()->KickProduct();
        }
        else
        {
            // Tell the game that no products should be kicked
            GetGame()->KickProduct(false);
        }

        // If the kick animation is done, reset values
        if (mKickTime >= mKickDuration)
        {
            mIsKicking = false;
            mKickTime = 0.0;
        }
    }

    // Only kick if the state has changed to one and the conveyor is moving
    if (mLastState != currentState && currentState == LogicState::One && GetGame()->IsConveyorMoving())
    {
        mIsKicking = true;
    }

    mLastState = currentState;
}

/**
 * If the wire is released over an input pin, connect the pins
 * @param vec The mouse position in virtual pixels
 */
void Sparty::Released(Vector vec)
{
    auto currentPin = GetGame()->GetCurrentPin();

    if (currentPin == nullptr)
    {
        return;
    }

    if (mInputPin.HitTest(vec))
    {
        mInputPin.SetIncomingPin(currentPin);
        SetCurrentPin(nullptr);
    }
}

/**
 * Sets the currently selected pin
 * @param pin The current pin
 */
void Sparty::SetCurrentPin(Pin *pin)
{
    auto currentPin = GetGame()->GetCurrentPin();

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
