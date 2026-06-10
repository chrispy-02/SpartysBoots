/**
 * @file Sensor.cpp
 * @author Zaid Qourah
 */

#include "pch.h"

#include <wx/graphics.h>
#include "Product.h"
#include "Sensor.h"

#include "SensorRangeVisitor.h"

/// The image file for the sensor cable
const std::wstring SensorCableImage = L"sensor-cable.png";

/// The image file for the sensor camera
const std::wstring SensorCameraImage = L"sensor-camera.png";

/// How far below Y location of the sensor system is the panel top?
const int PanelOffsetY = 87;

/// Range where a product is viewed by the sensor relative to the Y coordinate of the sensor.
const int SensorRange[] = {-40, 25}; // changed from 15 to 25 to expand the sensor range downward to fix a bug on our level 7

/// How much space for each property
/// @return None
const wxSize PropertySize(100, 40);

/// Maximum number of output pins supported
const int MaxOutputPins = 3;

/// Size of a shape as a property in virtual pixels
const double PropertyShapeSize = 32;

/// The background color to draw the sensor panel
const wxColour PanelBackgroundColor = ColorGray;

/// Space between pins
const double PinSpacing = 20.0;

/// The distance between the cable and the property pin
const int PropertyGapDistance = 5;

/// Scale the size of the diamond down to fit in property rectangle
const double DiamondScaleFactor = 0.75;

/**
 * Constructor for the Sensor
 * @param game The game instance
 */
Sensor::Sensor(Game *game) : Item(game, SensorCameraImage) {}


/**
 * Load the sensor from XML.
 * @param node The XML node containing sensor information
 */
void Sensor::XmlLoad(wxXmlNode *node)
{
    double x = stod(node->GetAttribute(L"x").ToStdString());
    double y = stod(node->GetAttribute(L"y").ToStdString());
    SetPosition(Vector(x, y));

    // stuff that was in LevelLoader
    std::vector<Product::Properties> properties;
    wxXmlNode *child = node->GetChildren();
    while (child)
    {
        std::wstring propertyName = child->GetName().ToStdWstring();
        if (Product::NamesToProperties.find(propertyName) != Product::NamesToProperties.end())
        {
            properties.push_back(Product::NamesToProperties.at(propertyName));
        }
        child = child->GetNext();
    }

    // Initialize output pins and map them to properties
    for (int i = 0; i < properties.size(); ++i)
    {
        // Each property rectangle is PropertySize tall
        double yOffset = i * (PropertySize.GetY());
        Vector relativePos(x - PropertyGapDistance, yOffset + PanelOffsetY);

        mPropertyRelativePositions.push_back(relativePos);

        auto property = properties[i];
        auto pin = std::make_shared<Pin>(PinType::Output, this);
        pin->SetPosition(relativePos + GetPosition() + Vector(PropertySize.GetWidth(), PropertySize.GetHeight() / 2));
        mOutputPins.push_back(pin);
        mPropertyToPinMap[property] = pin;
    }
}


/**
 * Draw the sensor on the screen.
 * @param graphics The graphics context to draw on
 */
void Sensor::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Draw lines indicating sensor range
    // graphics->SetPen(*wxRED_PEN);
    // graphics->StrokeLine(GetX() - 100, GetY() + SensorRange[0], GetX() + 100, GetY() + SensorRange[0] );
    // graphics->StrokeLine(GetX() - 100, GetY() + SensorRange[1], GetX() + 100, GetY() + SensorRange[1] );

    auto assetManager = GetGame()->GetAssetManager();

    // Create the image objects
    auto cableImage = assetManager.GetImage(SensorCableImage, graphics);
    auto cameraImage = assetManager.GetImage(SensorCameraImage, graphics);

    auto width = cableImage->GetWidth();
    auto height = cableImage->GetHeight();

    double drawYPos = GetPosition().GetY() - height / 2;
    double drawXPos = GetPosition().GetX() - width / 2;

    // Draw the bitmaps
    graphics->DrawBitmap(cableImage->GetBitmap(), drawXPos, drawYPos, width, height);
    graphics->DrawBitmap(cameraImage->GetBitmap(), drawXPos, drawYPos, width, height);

    // Draw output pins
    for (auto pin : mOutputPins)
    {
        // Draw the pin using its own Draw method
        pin->Draw(graphics);
    }

    // Draw the sensor panel background
    graphics->SetBrush(PanelBackgroundColor);
    graphics->SetPen(*wxTRANSPARENT_PEN);

    Vector panelPos = GetPosition() + Vector(0, -PanelOffsetY);
    graphics->DrawRectangle(panelPos.GetX(), panelPos.GetY(), PropertySize.GetX(),
                            mOutputPins.size() * (PropertySize.GetY()));

    // Draw property rectangles
    for (int i = 0; i < mOutputPins.size(); ++i)
    {
        // Retrieve the property associated with this pin
        Product::Properties property = Product::Properties::None;
        for (const auto &[prop, pPin] : mPropertyToPinMap)
        {
            if (pPin == mOutputPins[i])
            {
                property = prop;
                break;
            }
        }

        Vector absolutePropPos = GetPosition() + mPropertyRelativePositions[i];

        // Determine if the property is a color or content type
        auto typeIt = Product::PropertiesToTypes.find(property);

        Product::Types propType = typeIt->second;

        // Set brush based on property type
        wxBrush brush;
        if (propType == Product::Types::Color)
        {
            // Set brush color based on property
            if (property == Product::Properties::Red)
                brush.SetColour(ColorRed); // Red
            else if (property == Product::Properties::Green)
                brush.SetColour(ColorGreen); // Green
            else if (property == Product::Properties::Blue)
                brush.SetColour(ColorBlue); // Blue
            else if (property == Product::Properties::White)
                brush.SetColour(*wxWHITE);
            else
                brush.SetColour(ColorGray); // Default Grey for unknown colors
        }
        else if (propType == Product::Types::Content)
        {
            // Set a neutral color for content properties
            brush.SetColour(ColorLightGray); // Light Grey
        }
        else if (propType == Product::Types::Shape)
        {
            // Set a distinct color or keep it transparent for shape properties
            brush.SetColour(ColorLessLightGray); // Light Grey for shapes
        }
        else
        {
            // Default color
            brush.SetColour(ColorGray);
        }

        graphics->SetBrush(brush);
        graphics->SetPen(*wxBLACK_PEN);
        graphics->DrawRectangle(absolutePropPos.GetX(), absolutePropPos.GetY(), PropertySize.GetX(),
                                PropertySize.GetY());

        // If the property is of type Content, draw the associated image inside the rectangle
        if (propType == Product::Types::Content)
        {
            auto itImage = Product::PropertiesToContentImages.find(property);
            if (itImage != Product::PropertiesToContentImages.end())
            {
                std::wstring imageFile = itImage->second;
                auto contentImage = assetManager.GetImage(imageFile, graphics);

                // Calculate image position to center it within the rectangle
                double imageX = absolutePropPos.GetX() + (PropertySize.GetX() - PropertyShapeSize) / 2.0;
                double imageY = absolutePropPos.GetY() + (PropertySize.GetY() - PropertyShapeSize) / 2.0;

                graphics->DrawBitmap(contentImage->GetBitmap(), imageX, imageY, PropertyShapeSize, PropertyShapeSize);
            }
        }
        else if (propType == Product::Types::Shape)
        {
            // Draw the corresponding shape inside the rectangle
            double shapeSize = PropertyShapeSize; // Size of the shape within the rectangle
            double shapeX = absolutePropPos.GetX() + (PropertySize.GetX() - shapeSize) / 2.0;
            double shapeY = absolutePropPos.GetY() + (PropertySize.GetY() - shapeSize) / 2.0;

            // Set pen and brush for the shape
            graphics->SetPen(*wxBLACK_PEN);
            graphics->SetBrush(*wxWHITE);

            if (property == Product::Properties::Square)
            {
                graphics->DrawRectangle(shapeX, shapeY, shapeSize, shapeSize);
            }
            else if (property == Product::Properties::Circle)
            {
                graphics->DrawEllipse(shapeX, shapeY, shapeSize, shapeSize);
            }
            else if (property == Product::Properties::Diamond)
            {
                // Draw a diamond by rotating a square
                graphics->PushState();
                graphics->Translate(shapeX + shapeSize / 2.0, shapeY + shapeSize / 2.0);

                // Scale down to make the diamond smaller within the rectangle
                graphics->Scale(DiamondScaleFactor, DiamondScaleFactor);

                graphics->Rotate(M_PI / 4); // 45 degrees rotation
                graphics->DrawRectangle(-shapeSize / 2.0, -shapeSize / 2.0, shapeSize, shapeSize);
                graphics->PopState();
            }
        }
    }
}

/**
 * Update function for the Sensor
 * @param elapsed How much time has passed since the last update
 */
void Sensor::Update(double elapsed)
{
    // Detect the product in range
    auto detectedProduct = DetectProduct();

    // If this output is different from the last output, set it
    if (detectedProduct != mLastDetectedProduct)
    {
        mLastDetectedProduct = detectedProduct;
    }

    // Update the output pins based on the detected product
    ComputeState();
}

/**
 * Detect if a product is in range of the sensor.
 * @return The product detected by the sensor, or nullptr if no product is detected.
 */
Product *Sensor::DetectProduct()
{
    // Sensor's Y position
    auto sensorY = GetPosition().GetY();
    auto size = Product::Size / 2;

    SensorRangeVisitor visitor(sensorY + SensorRange[0] - size, sensorY + SensorRange[1] + size);

    GetGame()->AcceptVisitor(&visitor);

    return visitor.GetDetectedProduct();
}


/**
 * Computes the state for each of the Sensor's output pins
 */
void Sensor::ComputeState()
{
    // Reset all output pins to LogicState::Zero
    for (auto &pin : mOutputPins)
    {
        pin->SetState(LogicState::Zero);
    }

    if (mLastDetectedProduct != nullptr)
    {
        // Activate pins corresponding to the product's properties
        const auto &productProperties = mLastDetectedProduct->GetProperties();

        for (const auto &property : productProperties)
        {
            auto it = mPropertyToPinMap.find(property);
            if (it != mPropertyToPinMap.end())
            {
                it->second->SetState(LogicState::One);
            }
        }
    }
}

/**
 * Handle the dragging of the sensor.
 * @param mousePos The position of the mouse in virtual pixels.
 */
void Sensor::Dragged(Vector mousePos)
{
    auto currentPin = GetCurrentPin();

    if (currentPin != nullptr)
    {
        currentPin->SetWireEnd(mousePos);
    }
}

/**
 * Detects whether the mouse clicked this object
 *
 * @param vec The position to test at (in virtual pixels)
 * @return Whether the mouse clicked this object
 */
bool Sensor::HitTest(Vector vec)
{
    // Iterate through all of the output pins
    for (auto &pin : mOutputPins)
    {
        // If output pin was clicked, start dragging a wire from it
        if (pin->HitTest(vec))
        {
            pin->SetWireEnd(vec);
            SetCurrentPin(pin.get());
            return true;
        }
    }

    return false;
}

/**
 * Set the current pin being manipulated.
 * @param pin The pin that is currently being dragged or set.
 */
void Sensor::SetCurrentPin(Pin *pin)
{
    auto currentPin = GetCurrentPin();

    if (pin != nullptr)
    {
        pin->SetActive(true);
    }

    if (currentPin != nullptr && currentPin != pin)
    {
        currentPin->SetActive(false);
    }

    GetGame()->SetCurrentPin(pin);
}

