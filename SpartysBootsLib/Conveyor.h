/**
 * @file Conveyor.h
 * @author Bob Wilson
 *
 * Class for a Conveyor that moves the products
 */

#pragma once

#include "Game.h"
#include "Item.h"

/**
 * Class for a Conveyor
 */
class Conveyor : public Item
{
  private:
  public:
    /// Default constructor (disabled)
    Conveyor() = delete;

    /// Copy constructor (disabled)
    Conveyor(const Conveyor &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Conveyor &) = delete;

    Conveyor(Game *game);

    void XmlLoad(wxXmlNode *node) override;

    void Update(double elapsed) override;

    /**
     * Mouse click hit test on a Conveyor
     *
     * @param vec The hit position in virtual pixels
     * @return False because a Conveyor cannot be clicked on
     */
    bool HitTest(Vector vec) override { return false; }
};
