/**
 * @file LevelLoader.h
 * @author Bob Wilson
 *
 * A level loading class
 */

#pragma once

class Game; // forward reference

/**
 * A level loading class
 */
class LevelLoader
{
  private:
    /// The game this item belongs to
    Game *mGame;

  public:
    void XmlItem(wxXmlNode *node);

    void Load(const wxString &filename);

    LevelLoader(Game *game);
};
