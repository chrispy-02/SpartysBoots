/**
 * @file SpartysBootsApp.cpp
 * @author Zaid Qourah
 */
#include "pch.h"
#include "SpartysBootsApp.h"
#include <MainFrame.h>
/**
 * Initialize the application.
 * @return
 */
bool SpartysBootsApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // Add image type handlers
    wxInitAllImageHandlers();
    auto frame = new MainFrame();
    frame->Initialize();
    frame->Show(true);

    return true;
}