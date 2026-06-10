/**
 * @file Scoreboard.h
 * @author Christopher Nguyen
 *
 * Class that implements the Scoreboard.
 */

#ifndef SPARTYSBOOTS_SPARTYSBOOTSLIB_SCOREBOARD_H
#define SPARTYSBOOTS_SPARTYSBOOTSLIB_SCOREBOARD_H

#include <string>
#include <vector>
#include "Item.h"

/**
 * Class that represents the Scoreboard.
 */
class Scoreboard : public Item
{
  private:
    /// The instructions text lines
    std::vector<std::wstring> mInstructions;

  public:
    /// Default constructor (disabled)
    Scoreboard() = delete;

    /// Copy constructor (disabled)
    Scoreboard(const Scoreboard &) = delete;

    /// Assignment operator
    void operator=(const Scoreboard &) = delete;

    /// Constructor
    Scoreboard(Game *game);

    /// Load the attributes for an item node.
    void XmlLoad(wxXmlNode *node) override;

    /// Draw the scoreboard
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    /**
     * Mouse click hit test on a Scoreboard
     *
     * @param vec The hit position in virtual pixels
     * @return False because a Scoreboard cannot be clicked on
     */
    bool HitTest(Vector vec) override { return false; }
};

#endif // SPARTYSBOOTS_SPARTYSBOOTSLIB_SCOREBOARD_H
