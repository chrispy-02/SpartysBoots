/**
 * @file MainFrame.h
 * @author Zaid Qourah
 *
 * The MainFrame class for this application
 */

#ifndef SPARTYSBOOTS_SPARTYSBOOTSLIB_MAINFRAME_H
#define SPARTYSBOOTS_SPARTYSBOOTSLIB_MAINFRAME_H

class GameView;

/**
 * The top-level (main) frame of the application
 */
class MainFrame : public wxFrame
{

  private:
    /// View class for our Game
    GameView *mGameView = nullptr;
    void OnClose(wxCloseEvent &event);
    void OnAbout(wxCommandEvent &);
    void OnExit(wxCommandEvent &event);

  public:
    void Initialize();
};

#endif // SPARTYSBOOTS_SPARTYSBOOTSLIB_MAINFRAME_H
