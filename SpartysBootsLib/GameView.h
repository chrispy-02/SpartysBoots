/**
 * @file GameView.h
 * @author Zaid Qourah
 * @author Elijah Porter
 *
 * This is the game view for SpartysBoots. It handles the timing, interaction events, and painting for the game.
 */

#pragma once

#include <random>

#include "Game.h"

/**
 * View class for our game
 */
class GameView : public wxWindow
{
  private:
    void OnPaint(wxPaintEvent &event);
    void OnTimer(wxTimerEvent &event);

    /// The timer that allows for animation
    wxTimer mTimer;

    /// Stopwatch used to measure elapsed time
    wxStopWatch mStopWatch;

    /// The last stopwatch time
    long mTime = 0;

    /// An object that describes our game
    Game mGame;

    /// A pointer to the current grabbed item
    std::shared_ptr<Item> mGrabbedItem;

    /// The random number generator for this GameView
    std::mt19937 mRandomNumberGenerator;

  public:
    GameView();

    void Initialize(wxFrame *parent);
    void OnGateAdd(wxCommandEvent &e);
    void OnLevelLoad(wxCommandEvent &event);

    /**
     * Stop the timer so the window can close
     */
    void Stop() { mTimer.Stop(); }

    void OnLeftDown(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
    void OnMouseMove(wxMouseEvent &event);
};
