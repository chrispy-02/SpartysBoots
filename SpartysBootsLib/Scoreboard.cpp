/**
 * @file Scoreboard.cpp
 * @author Christopher Nguyen
 *
 * Implementation of the Scoreboard class.
 */

#include "pch.h"
#include "Scoreboard.h"
#include "Game.h"

using namespace std;

/// Size of the scoreboard in virtual pixels
/// @return None
const wxSize ScoreboardSize(380, 100);

/// The spacing from the scores to the instructions presentation in virtual pixels
const int SpacingScoresToInstructions = 40;

/// The spacing between the instruction lines in virtual pixels.
const int SpacingInstructionLines = 17;

/// The Default good score
const int DefaultGoodScore = 10;

/// The accent color for the scoreboard
/// @return None
const wxColour ScoreboardAccentColor = ColorGreen;

/// The font size for the scoreboard header
const int ScoreboardFontSize = 25;

/// The font size for the scoreboard instructions
const int InstructionsFontSize = 15;

/// The padding for the scoreboard
const int ScoreboardPadding = 10;

/// The score text's horizontal offset
const int ScoreOffsetX = 190;

/**
 * Constructor.
 * @param game The game this item belongs to
 */
Scoreboard::Scoreboard(Game* game) : Item(game, L"") // No image for the scoreboard
{
}

/**
 * Load the attributes for an item node.
 *
 * @param node The Xml node we are loading the item from
 */
void Scoreboard::XmlLoad(wxXmlNode* node)
{
    // Load x and y attributes
    double x = stod(node->GetAttribute(L"x", L"0").ToStdWstring());
    double y = stod(node->GetAttribute(L"y", L"0").ToStdWstring());
    SetPosition(Vector(x, y));

    // Load good and bad score increments
    int goodScore = DefaultGoodScore;
    int badScore = 0;

    node->GetAttribute(L"good", L"10").ToInt(&goodScore);
    node->GetAttribute(L"bad", L"0").ToInt(&badScore);

    // Set the good and bad scores in the Game
    GetGame()->SetGoodScore(goodScore);
    GetGame()->SetBadScore(badScore);

    // Parse the instructions text content and <br/> tags
    wxXmlNode* child = node->GetChildren();
    while (child)
    {
        if (child->GetType() == wxXML_TEXT_NODE)
        {
            // Get the text content
            std::wstring instructionLine = child->GetContent().ToStdWstring();
            mInstructions.push_back(instructionLine);
        }
        else if (child->GetName() == L"br")
        {
            // Line break, add an empty string to represent line break
            mInstructions.push_back(L"");
        }
        child = child->GetNext();
    }
}

/**
 * Draw the scoreboard
 * @param graphics Graphics context to draw on
 */
void Scoreboard::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Get the position
    double x = GetX();
    double y = GetY();

    // Draw the scoreboard background
    wxBrush backgroundBrush(*wxWHITE);
    graphics->SetBrush(backgroundBrush);
    graphics->SetPen(*wxTRANSPARENT_PEN);
    graphics->DrawRectangle(x, y, ScoreboardSize.GetWidth(), ScoreboardSize.GetHeight());

    // Draw the border
    wxPen borderPen(ScoreboardAccentColor, 1);
    graphics->SetPen(borderPen);
    graphics->SetBrush(*wxTRANSPARENT_BRUSH);
    graphics->DrawRectangle(x, y, ScoreboardSize.GetWidth(), ScoreboardSize.GetHeight());

    // Draw the scores
    auto scoreFont = graphics->CreateFont(ScoreboardFontSize, L"Arial", wxFONTFLAG_BOLD, ScoreboardAccentColor);
    graphics->SetFont(scoreFont);

    // Get the level score and game score from the game
    int levelScore = GetGame()->GetLevelScore();
    int gameScore = GetGame()->GetGameScore();

    auto lvlscoreText = wxString::Format(L"Level: %d", levelScore);
    auto gamescoreText = wxString::Format("Game: %d",gameScore);


    // Draw the score text
    graphics->DrawText(lvlscoreText, x + ScoreboardPadding, y + ScoreboardPadding);
    graphics->DrawText(gamescoreText, x + ScoreOffsetX, y + ScoreboardPadding);

    // Draw the instructions
    auto instructionsFont = graphics->CreateFont(InstructionsFontSize, L"Arial", wxFONTFLAG_BOLD, *wxBLACK);
    graphics->SetFont(instructionsFont);

    double instructionsY = y + ScoreboardPadding + SpacingScoresToInstructions;

    for (auto& line : mInstructions)
    {
        if (line.empty())
        {
            continue;
        }
        graphics->DrawText(line, x + ScoreboardPadding, instructionsY);
        instructionsY += SpacingInstructionLines;
    }
}
