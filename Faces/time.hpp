#pragma once

#include "Device/painter.hpp"
#include "Font/font5x7.hpp"
#include "Font/font8x8.hpp"
#include "Util/time.hpp"
#include "face.hpp"

namespace Faces
{

/**
 * Shows the current time.
 */
class Time : public Face
{
    using F = Font::Font5by7;
    Device::ScrollingDisplay *mDisplay;

    public:
    explicit Time(Device::ScrollingDisplay *display) : mDisplay(display)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        auto time = Util::getTime("%H:%M");
        mDisplay->clear();
        Device::Painter::writeText<F>(mDisplay, 0U, 0U, time);
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