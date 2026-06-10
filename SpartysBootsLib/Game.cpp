/**
 * @file Game.cpp
 * @author Zaid Qourah
 * @author Bob Wilson
 */

#include "pch.h"

#include <iostream>
#include <wx/filename.h>

#include "Game.h"
#include "Item.h"
#include "LastProcessedVisitor.h"
#include "Product.h"
#include "ScoringVisitor.h"

using namespace std;

/// The size of the void outside the game area
const double DeadZoneSize = 1000;

/// Images directory
const std::wstring DirectoryContainingImages = L"images/";

/// Level notices duration in seconds
const double LevelNoticeDuration = 2.0;

/// Size of notices displayed on screen in virtual pixels
const int NoticeSize = 100;

/// Color to draw the level notices
const auto LevelNoticeColor = ColorGreen;

/// Color to draw a rectangle enclosing the level notice text
/// @return None
const wxColour LevelNoticeBackground(255, 255, 255, 200);

/// The background color of the game area
/// @return None
const wxColour GameAreaBackgroundColor(230, 255, 230);

/// Padding to add to the left and right side of the level
/// notice background rectangle in virtual pixels
const double LevelNoticePadding = 20;

/// The time between products disappearing and displaying the level completed notice
const double LevelCompletedTimeout = 3.0;

/// The length of the level completed notice
const double LevelCompletedLength = 2.0;

/// The last level number in the Game
const int FinalLevelNumber = 8;

/**
 * Constructor
 */
Game::Game() : mLevelLoader(this) {}

/**
 * Draw the view on a graphics context
 * @param graphics Graphics context to draw on
 * @param width Width of the window
 * @param height Height of the window
 */
void Game::Draw(std::shared_ptr<wxGraphicsContext> graphics, int width, int height)
{
    // Calculate scaling factors based on dynamic level size
    double scaleX = double(width) / double(mWidth);
    double scaleY = double(height) / double(mHeight);
    mScale = std::min(scaleX, scaleY);

    // Calculate offsets to center the virtual pixels
    mOffset = Vector((width - mWidth * mScale) / 2.0, (height - mHeight * mScale) / 2.0);

    // Apply scaling and translation
    graphics->PushState();
    graphics->Translate(mOffset.GetX(), mOffset.GetY());
    graphics->Scale(mScale, mScale);

    // Set the game area background to light green
    graphics->SetBrush(GameAreaBackgroundColor);
    graphics->DrawRectangle(0, 0, mWidth, mHeight);


    for (auto item : mItems)
    {
        item->Draw(graphics);
    }

    DrawBackground(graphics);

    // Draw notices based on game state
    switch (mState)
    {
        case GameState::LevelStart:
            DrawLevelNotice(graphics);
            break;
        case GameState::LevelCompleted:
            DrawCompletedNotice(graphics);
            break;
        default:
            break;
    }

    graphics->PopState(); // Restore graphics context
}

/**
 * Draws a black border around the game area.
 *
 * @param graphics The GC to draw to
 */
void Game::DrawBackground(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Set the outer area to black
    wxBrush deadZoneBlack(*wxBLACK);
    graphics->SetBrush(deadZoneBlack);
    graphics->DrawRectangle(-DeadZoneSize, 0, DeadZoneSize, mHeight);
    graphics->DrawRectangle(mWidth, 0, DeadZoneSize, mHeight);
    graphics->DrawRectangle(-DeadZoneSize, -DeadZoneSize, 2 * DeadZoneSize + mWidth, DeadZoneSize);
    graphics->DrawRectangle(-DeadZoneSize, mHeight, 2 * DeadZoneSize + mWidth, DeadZoneSize);
}


/**
 * Update the class in time
 * @param elapsed Elapsed time in seconds
 */
void Game::Update(double elapsed)
{
    mStateTimer += elapsed;

    switch (mState)
    {
        case GameState::LevelStart:
            if (mStateTimer >= LevelNoticeDuration)
            {
                mState = GameState::Playing;
                mStateTimer = 0.0;
            }
            break;

        case GameState::Playing:
            // Update all items
            for (auto item : mItems)
            {
                item->Update(elapsed);
                ScoringVisitor scoringVisitor(this);
                AcceptVisitor(&scoringVisitor);
            }

            // Check if level end conditions are met
            if (IsLevelEndConditionMet())
            {
                mState = GameState::LevelEnd;
                mStateTimer = 0.0;
            }
            break;

        case GameState::LevelEnd:
            for (auto item : mItems)
            {
                item->Update(elapsed);
            }

            if (mStateTimer >= LevelCompletedTimeout)
            {
                mGameScore += mLevelScore;
                mState = GameState::LevelCompleted;
                mStateTimer = 0.0;
            }
            break;

        case GameState::LevelCompleted:
            if (mStateTimer >= LevelCompletedLength) // Display completed message for 2 seconds
            {
                LoadNextLevel();
                mState = GameState::LevelStart;
                mStateTimer = 0.0;
            }
            break;
    }
}

/**
 * Converts a Vector from window pixels to virtual pixels.
 *
 * @param vec The Vector in window pixels
 * @return A Vector in virtual pixels
 */
Vector Game::WindowToVirtual(const Vector &vec) { return (vec - mOffset) / mScale; }

/**
 * Converts a Vector from virtual pixels to window pixels.
 *
 * @param vec The Vector in virtual pixels
 * @return A Vector in pixels
 */
Vector Game::VirtualToWindow(const Vector &vec) { return (vec * mScale) + mOffset; }


/**
 * Set level size dynamically from level data
 * @param width Width of the level
 * @param height Height of the level
 */
void Game::SetLevelSize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

/**
 * Clear the level data.
 *
 * Deletes all known items in the level.
 */
void Game::Clear() { mItems.clear(); }


/**
 * Add an item to the game
 * @param item New item to add
 */
void Game::Add(std::shared_ptr<Item> item) { mItems.push_back(item); }


/**
 * Finds clicked Item from clicked location in virtual pixels
 * @param vec
 * @return
 */
std::shared_ptr<Item> Game::HitTest(Vector vec)
{
    // Iterate through all Items in reverse, starting at the end with the topmost Item
    for (auto i = mItems.rbegin(); i != mItems.rend(); i++)
    {
        // If this Item was clicked on, return a pointer to it
        if ((*i)->HitTest(vec))
        {
            return *i;
        }
    }

    // No items were clicked on, return nullptr
    return nullptr;
}


/**
 * Finds the Item that is crossing the beam
 * @param beamY The Y value of beam in virtual pixels
 * @return a shared pointer to the Item crossing the beam (or nullptr if none)
 */
std::shared_ptr<Item> Game::BeamTest(double beamY)
{
    for (auto i = mItems.rbegin(); i != mItems.rend(); i++)
    {
        if ((*i)->IsCrossingBeam(beamY))
        {
            return *i;
        }
    }
    return nullptr;
}


/**
 * Releases all items (tells them the mouse is no longer pressed)
 * @param vec The mouse position
 */
void Game::ReleaseAll(Vector vec)
{
    for (auto i = mItems.rbegin(); i != mItems.rend(); i++)
    {
        (*i)->Released(vec);
    }

    if (mCurrentPin != nullptr)
    {
        // Release the pin
        mCurrentPin->SetActive(false);
        mCurrentPin = nullptr;
    }
}

/**
 * Load a level
 *
 * @param filename The level's filename
 */
void Game::Load(const wxString &filename) { mLevelLoader.Load(filename); }

/**
 * Draws the Level Notice to the graphics context
 * @param graphics The graphics context
 */
void Game::DrawLevelNotice(std::shared_ptr<wxGraphicsContext> graphics)
{
    std::wstring levelText = L"Level " + std::to_wstring(mCurrentLevel) + L" Begin";

    // Create font
    wxFont font(NoticeSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    graphics->SetFont(font, LevelNoticeColor);

    // Measure text
    double textWidth, textHeight;
    graphics->GetTextExtent(levelText, &textWidth, &textHeight);

    // Calculate centered position
    double x = (mWidth - textWidth) / 2.0;
    double y = (mHeight - textHeight) / 2.0;

    // Draw background rectangle
    double rectX = x - LevelNoticePadding;
    double rectY = y - LevelNoticePadding;
    double rectWidth = textWidth + 2 * LevelNoticePadding;
    double rectHeight = textHeight + 2 * LevelNoticePadding;

    wxBrush backgroundBrush(LevelNoticeBackground);
    graphics->SetBrush(backgroundBrush);
    graphics->SetPen(*wxTRANSPARENT_PEN);
    graphics->DrawRectangle(rectX, rectY, rectWidth, rectHeight);

    // Draw the text
    graphics->SetBrush(*wxTRANSPARENT_BRUSH);
    graphics->DrawText(levelText, x, y);
}

/**
 * Draws the Level Completed notice on the graphics context
 * @param graphics The graphics context
 */
void Game::DrawCompletedNotice(std::shared_ptr<wxGraphicsContext> graphics)
{
    std::wstring completedText = L"Level Completed!";

    // Create font
    wxFont font(NoticeSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    graphics->SetFont(font, LevelNoticeColor);

    // Measure text
    double textWidth, textHeight;
    graphics->GetTextExtent(completedText, &textWidth, &textHeight);

    // Calculate centered position
    double x = (mWidth - textWidth) / 2.0;
    double y = (mHeight - textHeight) / 2.0;

    // Draw background rectangle
    double rectX = x - LevelNoticePadding;
    double rectY = y - LevelNoticePadding;
    double rectWidth = textWidth + 2 * LevelNoticePadding;
    double rectHeight = textHeight + 2 * LevelNoticePadding;

    wxBrush backgroundBrush(LevelNoticeBackground);
    graphics->SetBrush(backgroundBrush);
    graphics->SetPen(*wxTRANSPARENT_PEN);
    graphics->DrawRectangle(rectX, rectY, rectWidth, rectHeight);

    // Draw the text
    graphics->SetBrush(*wxTRANSPARENT_BRUSH);
    graphics->DrawText(completedText, x, y);
}

/**
 * Checks if the level end conditions are met
 * @return true if level should end, false otherwise
 */
bool Game::IsLevelEndConditionMet()
{
    LastProcessedVisitor visitor;

    AcceptVisitor(&visitor);

    // Whether the last product on the conveyor has been processed
    return visitor.HasLastBeenProcessed();
}

/**
 * Accepts an Item Visitor to all Items.
 * @param visitor The visitor to accept
 */
void Game::AcceptVisitor(ItemVisitor *visitor)
{
    // Iterate through the items in reverse.
    // This is necessary to fix a bug on level2 where the green Izzo was detected by the sensor
    // while the red football was still in front of the beam.
    for (auto item = mItems.rbegin(); item != mItems.rend(); item++)
    {
        (*item)->AcceptVisitor(visitor);
    }
}

/**
 * Load a specific level.
 * @param levelNumber The number of the level to load.
 */
void Game::LoadLevel(int levelNumber)
{
    // Update the current level number
    mCurrentLevel = levelNumber;
    std::wstring filename = L"levels/level" + std::to_wstring(mCurrentLevel) + L".xml";

    // Check if the level file exists
    wxFileName fn(filename);
    if (!fn.FileExists())
    {
        wxLogError(L"Level file '%s' does not exist.", filename);
        // Optionally, reset to a valid level or notify the user
        return;
    }

    // Clear existing items to prepare for the new level
    Clear();

    // Stop the conveyor belt
    mConveyorIsMoving = false;

    // Load the new level
    Load(filename);

    // Reset the state machine to LevelStart
    mState = GameState::LevelStart;
    mStateTimer = 0.0;
}

/**
 * Load the next level
 */
void Game::LoadNextLevel()
{
    // Increment the current level
    mCurrentLevel++;

    // If the current level exceeds 8, stop at level 8
    if (mCurrentLevel > FinalLevelNumber)
    {
        mCurrentLevel = FinalLevelNumber;
    }

    // Load the new level
    LoadLevel(mCurrentLevel);
}

/**
 * Start the conveyor and reset products to their original positions and settings
 */
void Game::ConveyorStart()
{
    mState = GameState::Playing;
    ResetLevelScore();

    // reset all items
    // products are reset to their original position and settings,
    // and FlipFLop gates are reset to their original settings
    for (auto item : mItems)
    {
        item->Reset();
    }

    // Start the conveyor
    mConveyorIsMoving = true;
}

/**
 * Reset the total game score.
 */
void Game::ResetGameScore()
{
    mGameScore = 0;
}

/**
     * Sets the game dimensions
     * @param x The x dimension
     * @param y The y dimension
     */
void Game::SetGameDimensions(int x, int y)
{

    mWidth = x;
    mHeight = y;

}

