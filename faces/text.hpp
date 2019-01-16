#pragma once

#include "face.hpp"
#include "font/font5x7.hpp"
#include "util/painter.hpp"

namespace Faces
{

/**
 * Shows the given string through a ScrollingDisplay.
 */
class Text : public Face
{
    Util::ScrollingDisplay *mDisplay;
    std::string mText;

    public:
    /**
     * Constructs a new text face.
     *
     * @param[in] display The pointer to the scrolling display.
     * @param[in] text    The string to show.
     */
    Text(Util::ScrollingDisplay *display, const std::string &text)
        : mDisplay(display), mText(text)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        mDisplay->clear();
        Util::Painter::writeText<Font::Font5by7>(
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
