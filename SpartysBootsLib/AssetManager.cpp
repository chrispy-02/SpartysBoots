/**
 * @file AssetManager.cpp
 * @author Elijah Porter
 */

#include "pch.h"

#include "AssetManager.h"
#include "Image.h"

using namespace std;

/**
 * Loads an Image if it hasn't already been loaded, otherwise returns the cached image.
 *
 * @param filename The filename of the image
 * @return a shared pointer to the Image object
 */
std::shared_ptr<Image> AssetManager::GetImage(std::wstring filename)
{
    auto iter = mImageCache.find(filename);
    if (iter != mImageCache.end())
    {
        return iter->second;
    }

    auto image = std::make_shared<Image>(filename);
    mImageCache[filename] = image;

    return image;
}

/**
 * Loads an Image and creates a Bitmap for it if it hasn't already been loaded, otherwise returns the cached image.
 *
 * @param filename The filename of the image
 * @param graphics The graphics context
 * @return a shared pointer to the Image object
 */
std::shared_ptr<Image> AssetManager::GetImage(std::wstring filename, std::shared_ptr<wxGraphicsContext> graphics)
{
    auto image = GetImage(filename);

    // Bitmap is only created if it doesn't already exist
    image->CreateBitmap(graphics);

    return image;
}

