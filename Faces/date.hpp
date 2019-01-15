#pragma once

#include "Util/painter.hpp"
#include "Util/time.hpp"
#include "face.hpp"

#include "Font/font5x7.hpp"

namespace Faces
{

/**
 * Clock face that displays the current date string, for example:
 * "Monday 1Dec"
 */
class Date : public Face
{
    /** Use 5x7 font */
    using F = Font::Font5by7;

    Util::ScrollingDisplay *mDisplay;

    public:
    explicit Date(Util::ScrollingDisplay *display) : mDisplay(display)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        auto time = Util::getTime("%A %e%b");
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
