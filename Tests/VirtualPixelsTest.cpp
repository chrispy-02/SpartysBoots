/**
 * @file VirtualPixelsTest.cpp
 * @author Zaid Qourah
 */

#include <pch.h>

#include "gtest/gtest.h"

#include <Game.h>
#include <Vector.h>
#include <wx/dc.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class TestFrame : public wxFrame {
public:
    TestFrame() : wxFrame(nullptr, wxID_ANY, "Test Frame", wxDefaultPosition, wxSize(1150, 800)) {
        // Set the background style as required for buffered paint
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &TestFrame::OnPaint, this);
    }

private:
    void OnPaint(wxPaintEvent& event) {
        // Simply clear the background
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();
    }
};

class VirtualPixelsTest : public ::testing::Test {
protected:
    Game mGame;

    // A dummy frame and graphics context for tests
    TestFrame* mFrame = nullptr;
    wxClientDC* mPaintDC = nullptr;
    std::shared_ptr<wxGraphicsContext> mGraphicsContext;

    void SetUp() override {
        // Initialize a dummy frame for creating a wxGraphicsContext
        mFrame = new TestFrame();

        // Create a bitmap and a memory DC
        mPaintDC = new wxClientDC(mFrame);
        mGraphicsContext = std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create(*mPaintDC));

        // Setup game with the initial level size
        mGame.SetLevelSize(1150, 800);  // Default level size
    }

    void TearDown() override {
        delete mFrame;
        delete mPaintDC;
    }
};

/**
 * Test that WindowToVirtual correctly converts window coordinates to virtual coordinates
 */
TEST_F(VirtualPixelsTest, TestWindowToVirtualConversion) {
    // Example window size (same as game space)
    Vector windowCoords(575, 400);  // Center of the window

    // Get virtual coordinates
    Vector virtualCoords = mGame.WindowToVirtual(windowCoords);

    // Assertions to check if the conversion is correct
    ASSERT_NEAR(virtualCoords.GetX(), 575, 1e-5);  // Expect no transformation
    ASSERT_NEAR(virtualCoords.GetY(), 400, 1e-5);  // Expect no transformation
}

/**
 * Test that VirtualToWindow correctly converts virtual coordinates to window coordinates
 */
TEST_F(VirtualPixelsTest, TestVirtualToWindowConversion) {
    // Example virtual coordinates (same as window space initially)
    Vector virtualCoords(575, 400);  // Center of the game space

    // Convert to window coordinates
    Vector windowCoords = mGame.VirtualToWindow(virtualCoords);

    // Assertions to check if the conversion is correct
    ASSERT_NEAR(windowCoords.GetX(), 575, 1e-5);  // Expect no transformation
    ASSERT_NEAR(windowCoords.GetY(), 400, 1e-5);  // Expect no transformation
}

/**
 * Test round-trip conversion between WindowToVirtual and VirtualToWindow
 */
TEST_F(VirtualPixelsTest, TestRoundTripConversion) {
    // Choose an arbitrary point in window space
    Vector originalWindowCoords(300, 500);

    // Convert to virtual space and back to window space
    Vector virtualCoords = mGame.WindowToVirtual(originalWindowCoords);
    Vector convertedBackToWindow = mGame.VirtualToWindow(virtualCoords);

    // Check if the round-trip conversion returns the original coordinates
    ASSERT_NEAR(originalWindowCoords.GetX(), convertedBackToWindow.GetX(), 1e-5);
    ASSERT_NEAR(originalWindowCoords.GetY(), convertedBackToWindow.GetY(), 1e-5);
}

/**
 * Test scaling behavior when window size differs from the game size
 */
TEST_F(VirtualPixelsTest, TestScalingAndOffset) {
    // Use the wxGraphicsContext to call Draw() with a smaller window size
    mGame.Draw(mGraphicsContext, 575, 400);  // Half the size of the default game space

    // Verify scaling factor
    ASSERT_NEAR(mGame.GetScale(), 0.5, 1e-5);

    // Verify offset (should be zero since the window size is perfectly half)
    ASSERT_NEAR(mGame.GetOffset().GetX(), 0, 1e-5);
    ASSERT_NEAR(mGame.GetOffset().GetY(), 0, 1e-5);
}

/**
 * Test that scaling is correct when the window size matches the game size
 */
TEST_F(VirtualPixelsTest, TestNoScalingWhenWindowMatchesGameSize) {
    // Use the wxGraphicsContext to call Draw() with the exact same size as the game space
    mGame.Draw(mGraphicsContext, 1150, 800);  // Same size as game space

    // Verify scaling factor (should be 1:1)
    ASSERT_NEAR(mGame.GetScale(), 1.0, 1e-5);

    // Verify no offset (since the window and game size match exactly)
    ASSERT_NEAR(mGame.GetOffset().GetX(), 0, 1e-5);
    ASSERT_NEAR(mGame.GetOffset().GetY(), 0, 1e-5);
}
