/**
 * @file Game.h
 * @author Zaid Qourah
 * @author Bob Wilson
 *
 * The Game class that defines how SpartysBoots functions
 */

#pragma once

#include "AssetManager.h"
#include "ItemVisitor.h"
#include "LevelLoader.h"
#include "Pin.h"
#include "Vector.h"

#include <memory>
#include <vector>

class Item; // forward reference

/// Possible States of the Game
enum class GameState
{
    LevelStart,
    Playing,
    LevelEnd,
    LevelCompleted
};

/**
 * Base class for our game
 */
class Game
{
  private:
    /// Offset Vector in window pixels
    Vector mOffset = Vector();

    /// The AssetManager for the game
    AssetManager mAssetManager;

    /// A container for all of the items to populate our game
    std::vector<std::shared_ptr<Item>> mItems;

    /// Scaling factor for virtual pixels
    double mScale = 1.0;

    /// Width of the game world
    int mWidth = 0;

    /// Height of the game world
    int mHeight = 0;

    /// The speed that the conveyor belt and products move down
    double mConveyorSpeed = 0;

    /// Whether or not the conveyor is moving
    bool mConveyorIsMoving = false;

    /// The currently selected pin
    Pin *mCurrentPin = nullptr;

    /// Level score
    int mLevelScore = 0;

    /// Game score
    int mGameScore = 0;

    /// Good score increment
    int mGoodScore = 0;

    /// Bad score increment
    int mBadScore = 0;

    /// The LevelLoader for the game
    LevelLoader mLevelLoader;

    /// Current state of the game
    GameState mState = GameState::LevelStart;

    /// Timer for the current state
    double mStateTimer = 0.0;

    /// Current level number
    int mCurrentLevel = 1;

    /// The Y coordinate of the beam in virtual pixels
    double mBeamY = 0.0;

    /// Whether the game should kick the current product off the conveyor
    bool mShouldKickProduct = false;

    /// How fast the product moves off the belt after being kicked in pixels per second
    double mKickSpeed = 0;

  public:
    Game();

    void Update(double elapsed);

    void DrawBackground(std::shared_ptr<wxGraphicsContext> graphics);

    void Draw(std::shared_ptr<wxGraphicsContext> graphics, int width, int height);

    void SetLevelSize(int width, int height);

    void Clear();

    void Add(std::shared_ptr<Item> item);

    /**
     * Is the conveyor moving?
     *
     * @return true if moving, false if not moving
     */
    bool IsConveyorMoving() const { return mConveyorIsMoving; }

    void ConveyorStart();

    /**
     * Stop the conveyor
     */
    void ConveyorStop() { mConveyorIsMoving = false; }

    /**
     * Gets whether the game is interactable
     * @return Whether the game is in an interactable state
     */
    bool IsInteractable() const { return mState == GameState::Playing || mState == GameState::LevelEnd; }

    /**
     * Sets whether products should be kicked
     * @param value Whether the product should be kicked
     */
    void KickProduct(bool value = true) { mShouldKickProduct = value; }

    /**
     *
     * @return Whether a product should be kicked
     */
    bool ShouldKickProduct() { return mShouldKickProduct; }

    std::shared_ptr<Item> HitTest(Vector vec);

    std::shared_ptr<Item> BeamTest(double beamY);

    void ReleaseAll(Vector vec);

    void Load(const wxString &filename);

    /**
     * Sets product kick speed
     * @param kickSpeed The kick speed
     */
    void SetKickSpeed(double kickSpeed) { mKickSpeed = kickSpeed; }

    /**
     * Gets product kick speed
     * @return The kick speed
     */
    double GetKickSpeed() { return mKickSpeed; }

    /**
     * Gets the pin that is currently being dragged
     *
     * @return The current pin being dragged
     */
    Pin *GetCurrentPin() { return mCurrentPin; }

    /**
     * Sets the pin that is currently being dragged
     *
     * @param pin The pin that is currently being dragged
     */
    void SetCurrentPin(Pin *pin) { mCurrentPin = pin; }


    /**
     * Set the conveyor belt speed
     *
     * @param speed Speed of the conveyor belt in pixels/second
     */
    void SetConveyorSpeed(double speed) { mConveyorSpeed = speed; }


    /**
     * Get the conveyor belt speed
     *
     * @return Speed of the conveyor belt in pixels/second
     */
    double GetConveyorSpeed() const { return mConveyorSpeed; }

    /**
     * Get the coordinate offset
     * @return offset width in pixels
     */
    Vector GetOffset() const { return mOffset; }

    Vector WindowToVirtual(const Vector &vec);

    Vector VirtualToWindow(const Vector &vec);

    /**
     * Gets the AssetManager associated with this Game.
     *
     * @return The AssetManager
     */
    AssetManager &GetAssetManager() { return mAssetManager; }

    /**
     * Get the scale for virtual pixel implementation
     * @return scale to fit
     */
    double GetScale() const { return mScale; }

    /**
     * Get the width of the game
     * @return Game width in pixels
     */
    int GetWidth() const { return mWidth; }

    /**
     * Get the height of the game
     * @return Game height in pixels
     */
    int GetHeight() const { return mHeight; }

    /**
     * Gets the Level Score
     * @return The Level Score
     */
    int GetLevelScore() const { return mLevelScore; }

    /**
     * Gets the Game Score
     * @return The Game Score
     */
    int GetGameScore() const { return mGameScore; }

    /**
     * Reset the level score
     */
    void ResetLevelScore() { mLevelScore = 0; }

    /**
     * Adds an amount to the Level Score
     * @param amount The amount to add
     */
    void AddToLevelScore(int amount) { mLevelScore += amount; }

    /**
     * Sets the good score increment
     * @param goodScore The good score increment
     */
    void SetGoodScore(int goodScore) { mGoodScore = goodScore; }

    /**
     * Sets the bad score increment
     * @param badScore The bad score increment
     */
    void SetBadScore(int badScore) { mBadScore = badScore; }

    /**
     * Gets the good score increment
     * @return The good score increment
     */
    int GetGoodScore() const { return mGoodScore; }

    /**
     * Gets the bad score increment
     * @return The bad Score increment
     */
    int GetBadScore() const { return mBadScore; }

    void SetGameDimensions(int x, int y);

    void DrawLevelNotice(std::shared_ptr<wxGraphicsContext> graphics);

    void DrawCompletedNotice(std::shared_ptr<wxGraphicsContext> graphics);

    void LoadNextLevel();

    void LoadLevel(int levelNumber);

    bool IsLevelEndConditionMet();

    void AcceptVisitor(ItemVisitor *visitor);

    /**
     * Set the Y coordinate of the Beam in virtual pixels
     * @param y The y value of the beam
     */
    void SetBeamY(double y) { mBeamY = y; }

    /**
     * Get the Y coordinate of the Beam in virtual pixels
     * @return The y coordinate of the beam
     */
    double GetBeamY() const { return mBeamY; }

    /**
     * The number of items in the level. This function is used in GameTest.
     * @return The number of items in the level
     */
    int NumberOfItems() const { return mItems.size(); }

    /**
     * Get all the items in the game.
     * @return A const reference to the vector of items.
     */
    const std::vector<std::shared_ptr<Item>> &GetItems() const { return mItems; }


    void ResetGameScore();
};
