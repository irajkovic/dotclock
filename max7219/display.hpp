#pragma once

#include <iostream>
#include <numeric>
#include <vector>

#include "device/display.hpp"
#include "device/spi.hpp"
#include "util/screenbuffer.hpp"

namespace Max7219
{

/**
 * The class provides the interface to the MAX7219 driven 8x8 led dot matrix
 * display.
 * Any number of daisy chained displays is supported, assuming that the
 * additional displays only increase the available width (ie. are added to the
 * right). Any actual screen geometry (the physical placement of the 8x8
 * segments) is not handled by this class. Currently, only left-to-right
 * arrangement of the displays is supported.
 */
class Display : public Device::Display
{
    public:
    /**
     * Constructs a new display object.
     *
     * @param[in] spi    The reference to the spi object.
     * @param[in] width  The width of the display, in pixels.
     */
    Display(Device::Spi &spi, unsigned int width) : mSpi(spi), mBuffer(width)
    {
        /* Prepare display for data writing */
        writeAll(Test::address, Test::off);
        writeAll(Shutdown::address, Shutdown::on);
        writeAll(Brightness::address, Brightness::lowest);
        writeAll(ScanLimit::address, ScanLimit::allDigits);
    }

    /**
     * Refreshes SPI interfaced dot matrix display with the data contained
     * in the screen buffer.
     */
    void refresh() override
    {
        auto height     = Util::ScreenBuffer::kHeight;
        auto segmentCnt = mBuffer.getSegmentCnt();

        for (uint8_t row = 1U; row <= height; row++) {
            for (uint8_t seg = 0U; seg < segmentCnt; seg++) {
                write(row, mBuffer.raw(height - row, seg), seg);
            }
        }

        mBuffer.dump();
    }

    /**
     * Clears the screen.
     */
    void clear() override
    {
        mBuffer.clear();
    }

    /**
     * Modifies pixel at the given coordinates. Coordinates are zero based.
     *
     * @param[in] x     The x coordinate of a pixel.
     * @param[in] y     The y coordinate of a pixel.
     * @param[in] pixel True to turn on the pixel, false to turn it off.
     */
    void putPixel(unsigned int x, unsigned int y, bool pixel) override
    {
        mBuffer.putBit(x, y, pixel);
    }

    /**
     * Turns on pixel at given coordinates. Coordinates are zero based.
     *
     * @param[in] x The x coordinate of a pixel.
     * @param[in] y The y coordinate of a pixel.
     */
    void setPixel(unsigned int x, unsigned int y) override
    {
        mBuffer.putBit(x, y, true);
    }

    /**
     * Turns off pixel at given coordinates. Coordinates are zero based.
     *
     * @param[in] x The x coordinate of a pixel.
     * @param[in] y The y coordinate of a pixel.
     */
    void resetPixel(unsigned int x, unsigned int y) override
    {
        mBuffer.putBit(x, y, false);
    }

    /**
     * Inserts column of bits to the right of the display, shifting the
     * contents of the display one pixel to the left.
     *
     * @param[in] column Byte holding 1x8 pixel column to be inserted.
     *
     * @return 1x8 pixel column that was pushed out to the left. If this
     *         value is fed back as a `column' parameter, the contents of
     *         the display will rotate cyclically.
     */
    uint8_t shiftLeft(uint8_t column) override
    {
        return mBuffer.shiftLeft(column);
    }

    private:
    /**
     * Writes the given value to all display segments.
     *
     * @tparam T      The type of the value, must be the size of a single
     * byte.
     * @param address The display register address.
     * @param value   The value to be written to the display register.
     */
    template <typename T> void writeAll(T address, T value)
    {
        auto segmentCnt             = mBuffer.getSegmentCnt();
        std::vector<uint8_t> buffer = {
            static_cast<uint8_t>(address),
            static_cast<uint8_t>(value),
        };

        for (auto i = 0U; i < segmentCnt; i++) {
            mSpi.write(buffer);
        }
    }

    /**
     * Writes the given value to a single display segment.
     *
     * @tparam T      The type of the value, must be the size of a single
     * byte.
     * @param address The display register address.
     * @param value   The value to be written to the display register.
     * @param segment The zero based index of a display segment.
     */
    template <typename T> void write(T address, T value, unsigned int segment)
    {
        /* Each segment expects 2 bytes long command */
        auto segmentCnt = mBuffer.getSegmentCnt();
        std::vector<uint8_t> buffer(segmentCnt * kCmdLen,
                                    static_cast<uint8_t>(NoOp::skip));

        auto ind      = (segmentCnt - segment - 1U) * kCmdLen;
        buffer[ind++] = static_cast<uint8_t>(address);
        buffer[ind]   = static_cast<uint8_t>(value);

        mSpi.write(buffer);
    }

    /** Reference to the SPI device. */
    Device::Spi &mSpi;

    /**
     * The length of an SPI command for a single segment (address + value
     * pair).
     */
    static const unsigned int kCmdLen = 2U;

    /**
     * Screen buffer.
     */
    Util::ScreenBuffer mBuffer;

    /** MAX7219 specific register constants, according to the data sheet. */

    enum class NoOp : uint8_t { skip = 0x00U };

    enum class DecodeMode : uint8_t {
        address    = 0x09U,
        disableBcd = 0x00U,
    };

    enum class Brightness : uint8_t {
        address = 0x0AU,
        lowest  = 0x00U,
    };

    enum class ScanLimit : uint8_t {
        address   = 0x0BU,
        allDigits = 0x0FU,
    };

    enum class Shutdown : uint8_t {
        address = 0x0CU,
        off     = 0x00U,
        on      = 0x01U,
    };

    enum class Test : uint8_t {
        address = 0x0FU,
        off     = 0x00U,
        on      = 0x01U,
    };
};

} /* namespace Max7219 */
