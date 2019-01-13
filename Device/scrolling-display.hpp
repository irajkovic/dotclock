#pragma once

#include <iostream>
#include <vector>

#include "Device/display.hpp"
#include "Device/screenbuffer.hpp"

namespace Device
{

/**
 * This class creates a virtual display that can show graphics of any width on
 * a width limited physical display, by scrolling the virtual display within
 * the actual display.
 */
class ScrollingDisplay : public Device::Display
{
    public:
    /**
     * Creates a new scrolling display.
     *
     * @param[in] phyDisp The pointer to the physical display.
     */
    explicit ScrollingDisplay(Display *phyDisp) : mPhyDisp(phyDisp), mBuffer(0U)
    {
        mPhyDisp->clear();
    }

    /**
     * Slides the contents displayed on the physical display by one column.
     * This method needs to be called repeatedly to create the illusion of
     * scrolling contents within the underlying physical display. The method
     * will also call refresh(), so the separate call to refresh the display is
     * not needed.
     *
     * @retval true  The contents of the virtual display has not scrolled fully.
     *               The user should continue to call this method in a loop.
     * @retval false The virtual display was fully shown. Next call to this
     *               method would start scrolling from the beginning.
     */
    bool slideIn()
    {
        mPhyDisp->shiftLeft(mBuffer.getColumn(mNextX));

        refresh();

        if (mNextX++ == mWidth) {
            mNextX = 0U;
        }

        return mNextX != 0U;
    }

    /**
     * Refreshes the screen.
     */
    void refresh() override
    {
        mPhyDisp->refresh();
    }

    /**
     * Sets the virtual display width to the zero, clearing internal buffers.
     * The display refresh() method is not called and must be called
     * explicitly if immediate change needs to be propagated to the display.
     */
    void clear() override
    {
        mBuffer.clear();

        mWidth = 0U;
        mNextX = 0U;
    }

    /**
     * Set pixels at given coordinate.
     *
     * @param[in] x     The x coordinate, zero based.
     * @param[in] y     The y coordinate, zero based.
     * @param[in] pixel True to enable pixel, false to disable it.
     */
    void putPixel(unsigned int x, unsigned int y, bool pixel) override
    {
        /** Dynamically increase internal buffer size, if needed */
        if (x >= mWidth) {
            mWidth = x;
            mBuffer.putBitExpanding(x, y, pixel);
        } else {
            mBuffer.putBit(x, y, pixel);
        }
    }

    /**
     * Sets pixels at given coordinate to on.
     *
     * @param[in] x     The x coordinate, zero based.
     * @param[in] y     The y coordinate, zero based.
     */
    void setPixel(unsigned int x, unsigned int y) override
    {
        putPixel(x, y, true);
    }

    /**
     * Sets pixels at given coordinate to off.
     *
     * @param[in] x     The x coordinate, zero based.
     * @param[in] y     The y coordinate, zero based.
     */
    void resetPixel(unsigned int x, unsigned int y) override
    {
        putPixel(x, y, false);
    }

    /**
     * Inserts the column to the virtual display, shifting the content of the
     * display one pixel to the left.
     *
     * @param[in] column The column to insert.
     *
     * @return The column that was pushed out.
     */
    uint8_t shiftLeft(uint8_t column) override
    {
        return mPhyDisp->shiftLeft(column);
    }

    private:
    /**
     * Pointer to the actual display that displays the data.
     */
    Display *mPhyDisp;

    /**
     * Width of the current (virtual) display.
     */
    unsigned int mWidth = 0U;

    /**
     * Index of the next column of pixels to be inserted into the actual
     * display.
     */
    unsigned int mNextX = 0U;

    /**
     * Screen buffer.
     */
    Device::ScreenBuffer mBuffer;
};

} /* namespace Device */
