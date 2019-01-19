#pragma once

#include <cinttypes>

namespace Device
{

namespace Display
{

/**
 * Abstract base class to define the display interface.
 */
class DisplayBase
{
    public:
    /**
     * Virtual destructor is a must for polymorphic base class.
     */
    virtual ~DisplayBase() = default;

    /**
     * Refreshes content of the display. This will flush the internal screen
     * buffer to the actual display device. All other display operations only
     * operate on the internal screen buffer and require call to this method
     * to make the changes apparent.
     */
    virtual void refresh() = 0;

    /**
     * Clears the content of the display.
     */
    virtual void clear() = 0;

    /**
     * Sets the pixel to the given value.
     *
     * @param[in] x     The X coordinate of the pixel.
     * @param[in] y     The Y coordinate of the pixel.
     * @param[in] pixel True/false to turn the pixel on/off.
     */
    virtual void putPixel(unsigned int x, unsigned int y, bool pixel) = 0;

    /**
     * Turns on the pixel to the given value.
     *
     * @param[in] x     The X coordinate of the pixel.
     * @param[in] y     The Y coordinate of the pixel.
     */
    virtual void setPixel(unsigned int x, unsigned int y) = 0;

    /**
     * Turns off the pixel to the given value.
     *
     * @param[in] x     The X coordinate of the pixel.
     * @param[in] y     The Y coordinate of the pixel.
     */
    virtual void resetPixel(unsigned int x, unsigned int y) = 0;

    /**
     * Shifts the contents of the screen one column to the left.
     *
     * @param[in] column The column of the pixels to insert at the rightmost
     *                   position.
     *
     * @return The column of the pixels that are pushed out.
     */
    virtual uint8_t shiftLeft(uint8_t column) = 0;
};

} // namespace Display

} // namespace Device
