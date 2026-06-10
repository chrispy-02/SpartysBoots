/**
 * @file AssetManager.h
 * @author Elijah Porter
 *
 * This class is used for managing the game assets (like images). It prevents an image from loading more than once per
 * level.
 */

#pragma once

#include <map>
#include <string>

#include "Image.h"

/**
 * The AssetManager class for managing images.
 */
class AssetManager
{
  private:
    /// The image cache, stored as a std::map. TA said this was OK.
    std::map<std::wstring, std::shared_ptr<Image>> mImageCache;

  public:
    std::shared_ptr<Image> GetImage(std::wstring filename, std::shared_ptr<wxGraphicsContext> graphics);

    std::shared_ptr<Image> GetImage(std::wstring filename);
};
