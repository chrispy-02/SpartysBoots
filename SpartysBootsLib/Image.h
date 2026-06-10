/**
 * @file Image.h
 * @author Elijah Porter
 *
 * The Image class creates a wxImage and wxGraphicsBitmap for a given image.
 */

#pragma once

#include <string>

/**
 * The Image class for creating wxImage and wxGraphicsBitmap objects from a filename.
 */
class Image
{
  private:
    /// The wxImage for this Image
    std::shared_ptr<wxImage> mWxImage;

    /// The wxGraphicsBitmap for this Image
    wxGraphicsBitmap mWxBitmap;

  public:
    Image(std::wstring filename);

    /**
     * Gets the wxGraphicsBitmap object associated with this Image.
     * @return The wxGraphicsBitmap object
     */
    wxGraphicsBitmap GetBitmap() { return mWxBitmap; }

    /**
     * Gets the wxImage object associated with this Image.
     *
     * @return The wxImage object
     */
    std::shared_ptr<wxImage> GetWxImage() { return mWxImage; }

    /**
     * Gets the width of the image.
     *
     * @return The image width
     */
    int GetWidth() { return mWxImage->GetWidth(); }

    /**
     * Gets the height of the image.
     *
     * @return The image height
     */
    int GetHeight() { return mWxImage->GetHeight(); }

    void CreateBitmap(std::shared_ptr<wxGraphicsContext> graphics);
};
