/**
 * @file ScoreboardTest.cpp
 * @author Zaid Qourah
 */

#include <pch.h>
#include <gtest/gtest.h>
#include <Scoreboard.h>
#include <Game.h>
#include <wx/xml/xml.h>
#include <wx/graphics.h>
#include <memory>

/**
 * Test fixture for the Scoreboard class.
 */
class ScoreboardTest : public ::testing::Test {
protected:
    // A pointer to the game instance needed for the Scoreboard
    Game* mGame = nullptr;
    // A pointer to the scoreboard instance being tested
    std::shared_ptr<Scoreboard> mScoreboard;

    // Called before each test case runs
    void SetUp() override {
        // Initialize the game
        mGame = new Game();

        // Create a scoreboard instance
        mScoreboard = std::make_shared<Scoreboard>(mGame);
    }

    // Called after each test case runs
    void TearDown() override {
        // Clean up any resources
        delete mGame;
        mScoreboard.reset();
    }

    // Helper function to load a sample XML for the scoreboard
    wxXmlNode* CreateSampleXmlNode() {
        auto* node = new wxXmlNode(wxXML_ELEMENT_NODE, "scoreboard");
        node->AddAttribute("x", "100");
        node->AddAttribute("y", "200");
        node->AddAttribute("good", "15");
        node->AddAttribute("bad", "-5");

        // Adding some instruction lines
        auto* instruction1 = new wxXmlNode(wxXML_TEXT_NODE, "");
        instruction1->SetContent("Score as many points as possible.");
        node->AddChild(instruction1);

        auto* brTag = new wxXmlNode(wxXML_ELEMENT_NODE, "br");
        node->AddChild(brTag);

        auto* instruction2 = new wxXmlNode(wxXML_TEXT_NODE, "");
        instruction2->SetContent("Avoid penalties.");
        node->AddChild(instruction2);

        return node;
    }
};

/**
 * Test that the Scoreboard::XmlLoad function correctly loads attributes and instructions from an XML node.
 */
TEST_F(ScoreboardTest, XmlLoad) {
    // Create a sample XML node with the expected data
    wxXmlNode* node = CreateSampleXmlNode();

    // Load the XML data into the scoreboard
    mScoreboard->XmlLoad(node);

    // Check the position of the scoreboard
    EXPECT_EQ(mScoreboard->GetX(), 100);
    EXPECT_EQ(mScoreboard->GetY(), 200);

    // Check the score values
    EXPECT_EQ(mScoreboard->GetGame()->GetGoodScore(), 15);
    EXPECT_EQ(mScoreboard->GetGame()->GetBadScore(), -5);

    // Cleanup
    delete node;
}

/**
 * Test that the Scoreboard::Draw method does not throw any errors when called.
 */
TEST_F(ScoreboardTest, Draw) {
    // Create a mock graphics context and wrap it in a shared_ptr
    wxGraphicsContext* rawGraphics = wxGraphicsContext::Create();
    std::shared_ptr<wxGraphicsContext> graphics(rawGraphics);

    // Ensure the Draw method doesn't throw or fail (functional test)
    EXPECT_NO_THROW(mScoreboard->Draw(graphics));

    // wxGraphicsContext is cleaned up automatically by shared_ptr
}

/**
 * Test to ensure correct loading of good and bad score increments.
 */
TEST_F(ScoreboardTest, ScoreIncrementLoad) {
    // Create a sample XML node with specific good and bad score attributes
    auto* node = new wxXmlNode(wxXML_ELEMENT_NODE, "scoreboard");
    node->AddAttribute("good", "20");
    node->AddAttribute("bad", "-10");

    // Load the XML data into the scoreboard
    mScoreboard->XmlLoad(node);

    // Check that the good and bad score increments are correctly set
    EXPECT_EQ(mScoreboard->GetGame()->GetGoodScore(), 20);
    EXPECT_EQ(mScoreboard->GetGame()->GetBadScore(), -10);

    // Cleanup
    delete node;
}



