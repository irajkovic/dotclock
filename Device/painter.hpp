#pragma once

#include <cinttypes>
#include <string>

#include "Font/font8x8.hpp"

namespace Device
{

namespace Painter
{

/**
 * Writes a single character to the display.
 *
 * @tparam Font    Font class to provide access to font pixmaps.
 * @tparam Display The display class.
 * @param[out] display The pointer to the display where text is to be drawn.
 * @param[in]  startX  The X coordinate of the symbol.
 * @param[in]  startY  The Y coordinate of the symbol.
 * @param[in]  letter  The ascii symbol to draw.
 *
 * @return The X coordinate where next character could be drawn.
 */
template <typename Font, typename Display>
unsigned int writeChar(Display *display,
                       unsigned int startX,
                       unsigned int startY,
                       uint8_t symbol)
{
    for (auto y = 0U; y < Font::height; y++) {
        for (auto x = 0U; x < Font::width; x++) {
            bool set = Font::at(x, y, symbol);
            display->putPixel(x + startX, y + startY, set);
        }
    }

    return startX + Font::width + Font::spacing;
}

/**
 * Writes a string to the display.
 *
 * @tparam Font    Font class to provide access to font pixmaps.
 * @tparam Display The display class.
 * @param[out] display The pointer to the display where text is to be drawn.
 * @param[in]  startX  The X coordinate of the symbol.
 * @param[in]  startY  The Y coordinate of the symbol.
 * @param[in]  text    The text to draw.
 *
 * @return The X coordinate where next string could be drawn.
 */
template <typename Font, typename Display>
unsigned int writeText(Display *display,
                       unsigned int startX,
                       unsigned int startY,
                       std::string text)
{
    for (auto ch : text) {
        startX =
            writeChar<Font>(display, startX, startY, static_cast<uint8_t>(ch));
    }

    return startX;
}

} /* namespace Painter */

} /* namespace Device */
