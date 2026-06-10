/**
 * @file Product.h
 * @author Zaid Qourah
 * @author Elijah Porter
 *
 * The product class for SpartysBoots
 */

#ifndef SPARTYSBOOTS_SPARTYSBOOTSLIB_PRODUCT_H
#define SPARTYSBOOTS_SPARTYSBOOTSLIB_PRODUCT_H

#include <map>
#include <string>
#include <vector>
#include "Item.h"
#include "Vector.h"
#include <iostream>

/**
 * Class for a Product on the conveyor.
 */
class Product : public Item
{
  public:
    /// Default product Size in pixels
    static const double Size;

    /// The possible product properties.
    enum class Properties
    {
        None,
        Red,
        Green,
        Blue,
        White,
        Square,
        Circle,
        Diamond,
        Izzo,
        Smith,
        Football,
        Basketball
    };

    /// The property types
    enum class Types
    {
        Color,
        Shape,
        Content
    };

    Product(Game *game);

    const std::vector<Properties> &GetProperties() const;

    void Update(double elapsed) override;

    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    void Reset() override;

    bool IsCrossingBeam(double beamY) override;

    void XmlLoad(wxXmlNode *node, Vector conveyorPosition);

    /// Mapping from XML strings for properties to Properties enum and type of the property.
    static const std::map<std::wstring, Properties> NamesToProperties;

    /// Mapping from a property to its type
    static const std::map<Properties, Types> PropertiesToTypes;

    /// Mapping from content properties to their associated image
    static const std::map<Properties, std::wstring> PropertiesToContentImages;

    bool IsProcessed() const;

    /**
     * Gets whether this is the last product on the conveyor
     * @return Whether this is the last product on the conveyor
     */
    bool GetIsLast() const { return mIsLast; }

    /**
     * Gets whether this Product has been kicked
     * @return Whether this Product was kicked from the conveyor
     */
    bool WasKicked() const { return mKicked; }

    /**
     * Sets whether this is the last product on the conveyor
     * @param value Whether this is the last product on the conveyor
     */
    void SetIsLast(bool value) { mIsLast = value; }

    /**
     * Accepts a visitor to this Product
     * @param visitor The visitor
     */
    void AcceptVisitor(ItemVisitor *visitor) override { visitor->VisitProduct(this); }

    /**
     * Mouse click hit test on a Product
     *
     * @param vec The hit position in virtual pixels
     * @return False because a Product cannot be clicked on
     */
    bool HitTest(Vector vec) override { return false; }

    /**
     * Check if the product is supposed to be kicked.
     * @return True if the product should be kicked.
     */
    bool ShouldBeKicked() const { return mSupposedToBeKicked; }

    /**
     * Set whether the product has been kicked.
     * @param kicked True if the product has been kicked.
     */
    void SetKicked(bool kicked) { mKicked = kicked; }

    /**
     * Set whether the product has passed the beam.
     * @param passed True if the product has passed the beam.
     */
    void SetPassedBeam(bool passed) { mPassedBeam = passed; }

    /**
     * Check if the product has passed the beam.
     * @return True if the product has passed the beam.
     */
    bool HasPassedBeam() const { return mPassedBeam; }

    /**
     * Set whether the product has been scored.
     * @param scored True if the product has been scored.
     */
    void SetHasBeenScored(bool scored) { mHasBeenScored = scored; }

    /**
     * Check if the product has been scored.
     * @return True if the product has been scored.
     */
    bool HasBeenScored() const { return mHasBeenScored; }


  private:
    /// The properties of the product
    std::vector<Properties> mProperties;

    /// The initial position of the product on the conveyor
    Vector mInitialPosition;

    /// Whether this product should be kicked off the conveyor
    bool mSupposedToBeKicked = false;

    /// Whether this product has been kicked off the conveyor
    bool mKicked = false;

    /// Whether this product has passed the beam
    bool mPassedBeam = false;

    /// Whether this is the last product on the conveyor
    bool mIsLast = false;

    /// Whether this product has been scored
    bool mHasBeenScored = false;
};

#endif // SPARTYSBOOTS_SPARTYSBOOTSLIB_PRODUCT_H
