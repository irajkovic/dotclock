#pragma once

#include "face.hpp"
#include "font/font5x7.hpp"
#include "util/painter.hpp"
#include "util/time.hpp"

namespace Faces
{

/**
 * Shows the current time.
 */
class Time : public Face
{
    using F = Font::Font5by7;
    Util::ScrollingDisplay *mDisplay;

    public:
    explicit Time(Util::ScrollingDisplay *display) : mDisplay(display)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        auto time = Util::getTime("%H:%M");
        mDisplay->clear();
        Util::Painter::writeText<F>(mDisplay, 0U, 0U, time);
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
