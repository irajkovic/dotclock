#pragma once

#include <cinttypes>
#include <iostream>
#include <vector>

namespace Util
{

/**
 * Abstracts low level bit operations on the display buffer and exposes
 * higher level index based interface.
 */
class ScreenBuffer
{

    public:
    constexpr static unsigned int kHeight = 8U;

    /**
     * Constructs a new object with the given width.
     *
     * @param[in] width Initial width of the screen, in pixels. This may
     *                  grow if putBitExpanding() is used.
     */
    explicit ScreenBuffer(unsigned int width)
        : mWidth(width), mSegmentCnt(width / 8U)
    {
        if (width % 8U != 0U) {
            throw std::invalid_argument("The width must be divisible by 8");
        }

        /* Setup screen buffer matrix */
        mBuffer.resize(kHeight);
        for (auto &row : mBuffer) {
            row.resize(mSegmentCnt);
        }
    }

    /**
     * Returns number of bytes needed to store a single row.
     *
     * @return The width of the display row, in bytes.
     */
    unsigned int getSegmentCnt()
    {
        return mSegmentCnt;
    }

    /**
     * Exposes low level bits. Useful when they are equivalent to the
     * hardware representation, so that they can be simply copied to the
     * target hardware.
     *
     * @param[in] y       The display row.
     * @param[in] segment The byte within the row.
     *
     * @return The asked byte.
     */
    uint8_t raw(unsigned int y, unsigned int segment)
    {
        return mBuffer[y][segment];
    }

    /**
     * Clears content of the internal screen buffer.
     */
    void clear()
    {
        for (auto &row : mBuffer) {
            std::fill(row.begin(), row.end(), 0U);
        }
    }

    /**
     * Dumps the screen buffer content to the console. Useful when
     * developing without access to the SPI dot matrix display.
     */
    void dump()
    {
        for (auto y = 0U; y < kHeight; y++) {
            for (auto x = 0U; x < mWidth; x++) {
                bool set = getBit(x, y);
                std::cout << (set ? 'X' : '-');
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    /**
     * Returns column of pixels at given coordinate, packed in a byte.
     *
     * @param[in] x The X coordinate of the column.
     *
     * @return The column of pixels.
     */
    uint8_t getColumn(unsigned int x)
    {
        auto ind     = getIndex(x);
        uint8_t mask = getMask(x);
        uint8_t col  = 0U;

        for (auto &row : mBuffer) {
            insertShifted(&col, (row[ind] & mask) != 0U);
        }

        return col;
    }

    /**
     * Modifies state of bit at point x,y.
     *
     * @param[in] x   The X coordinate of the point.
     * @param[in] y   The Y coordinate of the  point.
     * @param[in] bit True to set the point, false to turn it off.
     */
    void putBit(unsigned int x, unsigned int y, bool bit)
    {
        auto ind = getIndex(x);

        if (y < mBuffer.size() && ind < mBuffer[y].size()) {
            uint8_t *byte = &(mBuffer[y][ind]);
            bit ? setBit(byte, x) : resetBit(byte, x);
        }
    }

    /**
     * Modifies state of bit at point x,y. If the point X coordinate is outside
     * of the display buffer, the buffer size is increased to accommodate the
     * given point.
     *
     * @param[in] x   The X coordinate of the point.
     * @param[in] y   The Y coordinate of the  point.
     * @param[in] bit True to set the point, false to turn it off.
     */
    void putBitExpanding(unsigned int x, unsigned int y, bool bit)
    {
        auto ind = getIndex(x);

        /** Dynamically increase internal buffer size, if needed */
        if (y < mBuffer.size() && ind >= mBuffer[y].size()) {
            mSegmentCnt++;
            for (auto &row : mBuffer) {
                row.resize(ind + 1U);
            }
        }

        putBit(x, y, bit);
    }

    /**
     * Returns the state of the point at the given coordinates.
     * @param[in] x   The X coordinate of the point.
     * @param[in] y   The Y coordinate of the  point.
     *
     * @retval true  The point was set.
     * @retval false The point was not set, or point is outside of the display
     *               coordinates.
     */
    bool getBit(unsigned int x, unsigned int y)
    {
        auto ind = getIndex(x);

        if (y < mBuffer.size() && ind < mBuffer[y].size()) {
            return (mBuffer[y][ind] & getMask(x)) != 0U;
        }

        return false;
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
    uint8_t shiftLeft(uint8_t column)
    {
        uint8_t ret = 0U;

        for (auto row = 0U; row < mBuffer.size(); row++) {
            insertShifted(&ret, getCarry(mBuffer[row][0U]));

            for (auto seg = 0U; seg < mSegmentCnt; seg++) {
                bool carry = (seg < mSegmentCnt - 1U)
                                 ? getCarry(mBuffer[row][seg + 1U])
                                 : getCarry(column);

                insertShifted(&mBuffer[row][seg], carry);
            }

            column >>= 1U;
        }

        return ret;
    }

    private:
    /**
     * Vector used to store display buffer. Individual points (pixels) are
     * packed into byte, 8 points per byte.
     */
    std::vector<std::vector<uint8_t>> mBuffer;

    /**
     * The width of the display, in pixels.
     */
    unsigned int mWidth;

    /** Number of display segments. */
    unsigned int mSegmentCnt;

    /**
     * Maps horizontal coordinate of a pixel to an array index.
     *
     * @param[in] x The horizontal coordinate of a pixel.
     *
     * @return Index of an element in row buffer containing the value of
     * Xth pixel.
     */
    uint8_t getIndex(unsigned int x)
    {
        return x / 8U;
    }

    /**
     * Maps horizontal coordinate of a pixel to a bitmask that selects
     * the pixel.
     *
     * @param[in] x The horizontal coordinate of a pixel.
     *
     * @return Byte sized bitmap that selects corresponding pixel.
     */
    uint8_t getMask(unsigned int x)
    {
        return 1U << (x % 8U);
    }

    /**
     * Inserts point to the end of the given byte.
     *
     * @param[in] byte   Pointer to the byte where point is to be inserted.
     * @param[in] insert The value of the point.
     */
    void insertShifted(uint8_t *byte, bool insert)
    {
        *byte = (*byte >> 1U) | (insert << 7U);
    }

    /**
     * Get bit that would be pushed out if `insertShifted` were to be called
     * on the same bytee.
     *
     * @param[in] byte The pointer to the byte.
     *
     * @return Value of the point.
     */
    bool getCarry(uint8_t byte)
    {
        return (byte & 0x1U) != 0U;
    }

    /**
     * Sets Nth bit in byte to 0. If N is greater than 8, it is wrapped by mod
     * 8.
     *
     * @param[in] byte  The pointer to the byte.
     * @param[in] pixel Index of the bit to be set.
     */
    void setBit(uint8_t *byte, unsigned int pixel)
    {
        *byte |= getMask(pixel);
    }

    /**
     * Sets Nth bit in byte to 1. If N is greater than 8, it is wrapped by mod
     * 8.
     *
     * @param[in] byte  The pointer to the byte.
     * @param[in] pixel Index of the bit to be set.
     */
    void resetBit(uint8_t *byte, unsigned int pixel)
    {
        *byte &= ~getMask(pixel);
    }
};

} // namespace Util
