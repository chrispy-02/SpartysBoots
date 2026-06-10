/**
 * @file Sensor.h
 * @author Zaid Qourah
 *
 * The Sensor class that detects the types and properties of passing products.
 */

#ifndef SPARTYSBOOTS_SPARTYSBOOTSLIB_SENSOR_H
#define SPARTYSBOOTS_SPARTYSBOOTSLIB_SENSOR_H

#include <map>
#include <memory>
#include <string>
#include "Item.h"
#include "Pin.h"
#include "Product.h"
#include "Vector.h"

/**
 * Class for a Sensor
 */
class Sensor : public Item, public PinOwner
{
  private:
    /// List of output pins associated with the sensor
    std::vector<std::shared_ptr<Pin>> mOutputPins;

    /// Mapping from product properties to output pins
    std::map<Product::Properties, std::shared_ptr<Pin>> mPropertyToPinMap;

    /// The sensor camera position
    Vector mCameraPosition;

    /// The sensor cable position
    Vector mCablePosition;

    /// Relative positions for drawing property rectangles
    std::vector<Vector> mPropertyRelativePositions;

    /// Relative positions for drawing pins around the sensor
    std::vector<Vector> mPinRelativePositions;

    /// Keeps track of the last detected product
    Product* mLastDetectedProduct = nullptr;

  public:
    /// Default constructor (disabled)
    Sensor() = delete;

    /// Copy constructor (disabled)
    Sensor(const Sensor &) = delete;

    /// Assignment operator (disabled)
    void operator=(const Sensor &) = delete;

    Sensor(Game *game);

    void XmlLoad(wxXmlNode *node) override;

    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    Product *DetectProduct();

    void ComputeState() override;

    void Update(double elapsed) override;

    void Dragged(Vector mousePos) override;

    bool HitTest(Vector vec) override;

    void SetCurrentPin(Pin *pin);

    /**
     * Get the current pin being manipulated.
     * @return The currently active pin.
     */
    Pin *GetCurrentPin() { return GetGame()->GetCurrentPin(); }
};

#endif // SPARTYSBOOTS_SPARTYSBOOTSLIB_SENSOR_H
