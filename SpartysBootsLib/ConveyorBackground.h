/**
 * @file ConveyorBackground.h
 * @author Bob Wilson
 *
 * Class for a ConveyorBackground
 */

#pragma once

#include "Game.h"
#include "Item.h"

/**
 * Class for a ConveyorBackground
 */
class ConveyorBackground : public Item
{
  private:
  public:
    /// Default constructor (disabled)
    ConveyorBackground() = delete;

    /// Copy constructor (disabled)
    ConveyorBackground(const ConveyorBackground &) = delete;

    /// Assignment operator (disabled)
    void operator=(const ConveyorBackground &) = delete;

    ConveyorBackground(Game *game);

    void XmlLoad(wxXmlNode *node) override;

    /**
     * Mouse click hit test on a ConveyorBackground
     *
     * @param vec The hit position in virtual pixels
     * @return False because a ConveyorBackground cannot be clicked on
     */
    bool HitTest(Vector vec) override { return false; }
};
