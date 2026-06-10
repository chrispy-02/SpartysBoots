/**
 * @file GameView.cpp
 * @author Zaid Qourah
 * @author Elijah Porter
 */

#include "pch.h"

#include <wx/dcbuffer.h>

#include "GameView.h"
#include "GateAnd.h"
#include "GateDFlipFlop.h"
#include "GateNand.h"
#include "GateNot.h"
#include "GateOr.h"
#include "GateSRFlipFlop.h"
#include "LevelLoader.h"
#include "MainFrame.h"
#include "Vector.h"
#include "ids.h"

using namespace std;

/// Frame duration in milliseconds
const int FrameDuration = 30;

/// The spawn min position of a Gate in virtual pixels
const Vector GateSpawnPosMin(550, 200);

/// The spawn max position of a Gate in virtual pixels
const Vector GateSpawnPosMax(1000, 700);

/**
 * Constructor
 */
GameView::GameView()
{
    // Initialize the random number generator
    std::random_device rd;

    mRandomNumberGenerator = std::mt19937(rd());
}

/**
 * Initialization function
 * @param parent
 */
void GameView::Initialize(wxFrame *parent)
{
    // Create the window with wxFULL_REPAINT_ON_RESIZE to force repaint on resize
    Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Bind level loading menu events
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL0);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL1);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL2);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL3);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL4);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL5);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL6);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL7);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnLevelLoad, this, IDM_LEVEL8);

    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATEOR);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATEAND);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATENAND);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATENOT);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATESRFLIPFLOP);
    parent->Bind(wxEVT_COMMAND_MENU_SELECTED, &GameView::OnGateAdd, this, IDM_GATEDFLIPFLOP);

    // Bind the paint, timer, and mouse event
    Bind(wxEVT_PAINT, &GameView::OnPaint, this);
    Bind(wxEVT_TIMER, &GameView::OnTimer, this);
    Bind(wxEVT_LEFT_DOWN, &GameView::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &GameView::OnLeftUp, this);
    Bind(wxEVT_MOTION, &GameView::OnMouseMove, this);

    mTimer.SetOwner(this);
    mTimer.Start(FrameDuration);
    mStopWatch.Start();

    // load level 1 by default
    mGame.LoadLevel(1);
}

/**
 * Handles the Add Gate menu events
 *
 * @param event The gate add menu event
 */
void GameView::OnGateAdd(wxCommandEvent &event)
{
    // Define the ranges for the x and y positions
    std::uniform_int_distribution<int> xDist(GateSpawnPosMin.GetX(), GateSpawnPosMax.GetX());
    std::uniform_int_distribution<int> yDist(GateSpawnPosMin.GetY(), GateSpawnPosMax.GetY());

    // Generate a random position for the Gate
    auto spawnPosition = Vector(xDist(mRandomNumberGenerator), yDist(mRandomNumberGenerator));

    shared_ptr<Gate> gate;

    switch (event.GetId())
    {
        case IDM_GATEOR:
            gate = make_shared<GateOr>(&mGame);
            break;
        case IDM_GATEAND:
            gate = make_shared<GateAnd>(&mGame);
            break;
        case IDM_GATENAND:
            gate = make_shared<GateNand>(&mGame);
            break;
        case IDM_GATENOT:
            gate = make_shared<GateNot>(&mGame);
            break;
        case IDM_GATEDFLIPFLOP:
            gate = make_shared<GateDFlipFlop>(&mGame);
            break;
        case IDM_GATESRFLIPFLOP:
            gate = make_shared<GateSRFlipFlop>(&mGame);
            break;
    }

    gate->SetPosition(spawnPosition);
    mGame.Add(gate);
}

/**
 * Handle left mouse button down event
 *
 * @param event The mouse click event
 */
void GameView::OnLeftDown(wxMouseEvent &event)
{
    // the clicking things should only work if the level state is Playing or LevelEnd
    if (!mGame.IsInteractable())
    {
        return;
    }

    // Convert the event position in window space to game space (virtual pixels)
    auto eventVector = Vector(event.GetX(), event.GetY());
    auto virtualVec = mGame.WindowToVirtual(eventVector);


    // Set the grabbed item to whatever was clicked
    mGrabbedItem = mGame.HitTest(virtualVec);
}

/**
 * Handles the mouse move event for the GameView. Drags items.
 *
 * @param event The mouse event
 */
void GameView::OnMouseMove(wxMouseEvent &event)
{
    if (mGrabbedItem != nullptr)
    {
        // Convert the event position in window space to game space (virtual pixels)
        auto eventVector = Vector(event.GetX(), event.GetY());
        auto virtualVec = mGame.WindowToVirtual(eventVector);

        // Only drag if the game is in an interactable state
        if (event.LeftIsDown() && mGame.IsInteractable())
        {
            mGrabbedItem->Dragged(virtualVec);
        }
        else
        {
            mGame.ReleaseAll(virtualVec);
            mGrabbedItem = nullptr;
        }

        Refresh();
    }
}

/**
 * Handles the left mouse button up event for GaveView.
 *
 * @param event The mouse event
 */
void GameView::OnLeftUp(wxMouseEvent &event) { OnMouseMove(event); }

/**
 * Paint event, draws the window.
 * @param event Paint event object
 */
void GameView::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);

    // Set the entire background to black (this handles the "dead" space)
    wxBrush background(*wxBLACK);
    dc.SetBackground(background);
    dc.Clear(); // Clear the background with black

    // Compute the time that has elapsed since the last call to OnPaint.
    auto newTime = mStopWatch.Time();
    auto elapsed = (double)(newTime - mTime) * 0.001;
    mTime = newTime;

    // Update the game
    mGame.Update(elapsed);

    // Create a graphics context
    auto graphics = std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create(dc));
    if (!graphics)
    {
        wxLogError("Failed to create graphics context.");
        return;
    }

    // Get window dimensions
    wxRect rect = GetClientRect();

    // Draw the game
    mGame.Draw(graphics, rect.GetWidth(), rect.GetHeight());
}


/**
 * Handle timer events
 * @param event timer event
 */
void GameView::OnTimer(wxTimerEvent &event) { Refresh(); }

/**
 * The level load event handler.
 *
 * @param event The triggering event
 */
void GameView::OnLevelLoad(wxCommandEvent &event)
{
    wxString filename = L"levels/level";

    int levelNumber = 0;

    switch (event.GetId())
    {
        case IDM_LEVEL0:
            levelNumber = 0;
            break;
        case IDM_LEVEL1:
            levelNumber = 1;
            break;
        case IDM_LEVEL2:
            levelNumber = 2;
            break;
        case IDM_LEVEL3:
            levelNumber = 3;
            break;
        case IDM_LEVEL4:
            levelNumber = 4;
            break;
        case IDM_LEVEL5:
            levelNumber = 5;
            break;
        case IDM_LEVEL6:
            levelNumber = 6;
            break;
        case IDM_LEVEL7:
            levelNumber = 7;
            break;
        case IDM_LEVEL8:
            levelNumber = 8;
            break;
        default:
            return;
    }

    // Reset the total game score before loading the new level
    mGame.ResetGameScore();

    // Load the selected level using LoadLevel
    mGame.LoadLevel(levelNumber);

    // Refresh the view to reflect the new level
    Refresh();
}
