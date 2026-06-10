/**
 * @file ConveyorPanel.h
 * @author Bob Wilson
 * @author Elijah Porter
 *
 * Class for a ConveyorPanel
 */

#pragma once

#include "Game.h"
#include "Item.h"

/**
 * Class for a ConveyorPanel
 */
class ConveyorPanel : public Item
{
  private:
  public:
    /// Default constructor (disabled)
    ConveyorPanel() = delete;

    /// Copy constructor (disabled)
    ConveyorPanel(const ConveyorPanel &) = delete;

    /// Assignment operator (disabled)
    void operator=(const ConveyorPanel &) = delete;

    ConveyorPanel(Game *game);

    void XmlLoad(wxXmlNode *node, Vector);

    bool HitTest(Vector vec) override;
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;
};
