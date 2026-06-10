/**
 * @file SensorTest.cpp
 * @author Zaid Qourah
 */

#include <pch.h>
#include <gtest/gtest.h>
#include <Sensor.h>
#include <Game.h>
#include <Product.h>
#include <wx/xml/xml.h>
#include <wx/graphics.h>
#include <memory>

/**
 * Test fixture for the Sensor class.
 */
class SensorTest : public ::testing::Test {
protected:
    // A pointer to the game instance needed for the Sensor
    Game* mGame = nullptr;
    // A pointer to the sensor instance being tested
    std::shared_ptr<Sensor> mSensor;

    // Called before each test case runs
    void SetUp() override {
        // Initialize the game
        mGame = new Game();

        // Create a sensor
        mSensor = std::make_shared<Sensor>(mGame);
    }

    // Called after each test case runs
    void TearDown() override {
        // Clean up any resources
        delete mGame;
        mSensor.reset();
    }

    // Helper function to load a sample XML for the sensor
    wxXmlNode* CreateSampleXmlNode() {
        auto* node = new wxXmlNode(wxXML_ELEMENT_NODE, "sensor");
        node->AddAttribute("x", "100");
        node->AddAttribute("y", "200");

        // Adding some properties (e.g., red and square)
        auto* property1 = new wxXmlNode(wxXML_ELEMENT_NODE, "red");
        node->AddChild(property1);

        auto* property2 = new wxXmlNode(wxXML_ELEMENT_NODE, "square");
        node->AddChild(property2);

        return node;
    }
};


/**
 * Test that the Sensor::XmlLoad function correctly loads attributes and properties from an XML node.
 */
TEST_F(SensorTest, XmlLoad) {
    // Create a sample XML node with the expected data
    wxXmlNode* node = CreateSampleXmlNode();

    // Load the XML data into the sensor
    mSensor->XmlLoad(node);

    // Check the position of the sensor
    EXPECT_EQ(mSensor->GetX(), 100);
    EXPECT_EQ(mSensor->GetY(), 200);

    // Cleanup
    delete node;
}

/**
 * Test that the Sensor::Draw method does not throw any errors when called.
 */
TEST_F(SensorTest, Draw) {
    // Create a mock graphics context and wrap it in a shared_ptr
    wxGraphicsContext* rawGraphics = wxGraphicsContext::Create();
    std::shared_ptr<wxGraphicsContext> graphics(rawGraphics);

    // Ensure the Draw method doesn't throw or fail (functional test)
    EXPECT_NO_THROW(mSensor->Draw(graphics));

    // wxGraphicsContext is cleaned up automatically by shared_ptr
}
