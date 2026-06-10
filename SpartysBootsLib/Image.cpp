/**
 * @file Image.cpp
 * @author Elijah Porter
 */

#include "pch.h"

#include <string>

#include "Image.h"

using namespace std;

/**
 * Loads an image if it has not already been loaded.
 *
 * @param filename The filename of the image
 */
Image::Image(wstring filename) { mWxImage = make_shared<wxImage>(L"images/" + filename); }

/**
 * Creates a bitmap for the image if one hasn't already been created.
 *
 * @param graphics The graphics context
 */
void Image::CreateBitmap(shared_ptr<wxGraphicsContext> graphics)
{
    if (mWxBitmap.IsNull())
    {
        mWxBitmap = graphics->CreateBitmapFromImage(*mWxImage);
    }
}
