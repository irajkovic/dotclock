#pragma once

#include "Device/painter.hpp"
#include "Font/font5x7.hpp"
#include "Font/font8x8.hpp"
#include "face.hpp"

namespace Faces
{

/**
 * Shows the given string through a ScrollingDisplay.
 */
class Text : public Face
{
    Device::ScrollingDisplay *mDisplay;
    std::string mText;

    public:
    /**
     * Constructs a new text face.
     *
     * @param[in] display The pointer to the scrolling display.
     * @param[in] text    The string to show.
     */
    Text(Device::ScrollingDisplay *display, const std::string &text)
        : mDisplay(display), mText(text)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        mDisplay->clear();
        Device::Painter::writeText<Font::Font5by7>(
            mDisplay, 0U, 0U, mText.c_str());
    }

    /**
     * @see Face::run()
     */
    bool run() override
    {
        return mDisplay->slideIn();
    }
};
} // namespace Faces
