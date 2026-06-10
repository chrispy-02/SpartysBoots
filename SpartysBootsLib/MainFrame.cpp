/**
 * @file MainFrame.cpp
 * @author Zaid Qourah
 * @author Bob Wilson
 */

#include "pch.h"

#include "MainFrame.h"
#include "GameView.h"
#include "ids.h"

/// The default window size
/// @return None
const wxSize DefaultWindowSize(1200, 900);

/**
 * Initialize the MainFrame window.
 */
void MainFrame::Initialize()
{
    // Create the main frame window.
    // 1200x900 makes it the exact same size as the provided sample executable.
    Create(nullptr, wxID_ANY, L"Sparty's Boots",
           wxDefaultPosition, DefaultWindowSize);

    auto sizer = new wxBoxSizer(wxVERTICAL);

    mGameView = new GameView();
    mGameView->Initialize(this);

    sizer->Add(mGameView, 1, wxEXPAND | wxALL);

    SetSizer(sizer);
    Layout();

    auto menuBar = new wxMenuBar( );

    auto fileMenu = new wxMenu();
    auto levelMenu = new wxMenu();
    auto gateMenu = new wxMenu();
    auto helpMenu = new wxMenu();

    menuBar->Append(fileMenu, L"&File" );
    menuBar->Append(levelMenu, L"&Level");
    menuBar->Append(gateMenu, L"&Gates");
    menuBar->Append(helpMenu, L"&Help");

    fileMenu->Append(wxID_EXIT, L"E&xit\tAlt-X", L"Quit this program");
    helpMenu->Append(wxID_ABOUT, L"&About\tF1", L"Show about dialog");
    levelMenu->Append(IDM_LEVEL0, L"Level &0", L"Load Level 0");
    levelMenu->Append(IDM_LEVEL1, L"Level &1", L"Load Level 1");
    levelMenu->Append(IDM_LEVEL2, L"Level &2", L"Load Level 2");
    levelMenu->Append(IDM_LEVEL3, L"Level &3", L"Load Level 3");
    levelMenu->Append(IDM_LEVEL4, L"Level &4", L"Load Level 4");
    levelMenu->Append(IDM_LEVEL5, L"Level &5", L"Load Level 5");
    levelMenu->Append(IDM_LEVEL6, L"Level &6", L"Load Level 6");
    levelMenu->Append(IDM_LEVEL7, L"Level &7", L"Load Level 7");
    levelMenu->Append(IDM_LEVEL8, L"Level &8", L"Load Level 8");

    gateMenu->Append(IDM_GATEAND, L"AND", L"Add an AND Gate");
    gateMenu->Append(IDM_GATEOR, L"OR", L"Add an OR Gate");
    gateMenu->Append(IDM_GATENOT, L"NOT", L"Add a NOT Gate (inverter)");
    gateMenu->Append(IDM_GATENAND, L"NAND", L"Add a NAND Gate");
    gateMenu->Append(IDM_GATESRFLIPFLOP, L"SR Flip Flop", L"Add a Set/Reset Flip Flop");
    gateMenu->Append(IDM_GATEDFLIPFLOP, L"D Flip Flop", L"Add a D Flip Flop");

    // Set the menu bar for the frame
    SetMenuBar(menuBar);

    // Create a status bar
    CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

/**
 * Exit menu option handlers
 * @param event
 */
void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

/**
 * Application about box menu handler
 */
void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(L"Sparty's Boots\n",
                 L"Team Rheostat",
                 wxOK,
                 this);
}

/**
 * Handle a close event. Stop the animation and destroy this window.
 * @param event The Close event
 */
void MainFrame::OnClose(wxCloseEvent& event)
{
    mGameView->Stop();
    Destroy();
}

