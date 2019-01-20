#pragma once

#include "face.hpp"
#include "font/font5x7.hpp"
#include "util/painter.hpp"

#include <fstream>
#include <streambuf>

namespace Faces
{

/**
 * Shows the given string through a ScrollingDisplay.
 */
class File : public Face
{
    Util::ScrollingDisplay *mDisplay;
    std::string mPath;
    std::string mErrorStr;

    public:
    /**
     * Constructs a new text face.
     *
     * @param[in] display  The pointer to the scrolling display.
     * @param[in] path     The path of the file to load.
     * @param[in] errorStr The string to show if file cannot be loaded.
     */
    File(Util::ScrollingDisplay *display,
         const std::string &path,
         const std::string &errorStr = "---")
        : mDisplay(display), mPath(path), mErrorStr(errorStr)
    {
    }

    /**
     * @see Face::prepare()
     */
    void prepare() override
    {
        // Load the whole file into string
        std::ifstream fileStream(mPath);
        std::string text;

        if (fileStream.good()) {
            text = std::string{std::istreambuf_iterator<char>(fileStream),
                               std::istreambuf_iterator<char>()};

            // Remove last character, as stream includes EOF.
            text.pop_back();
        } else {
            text = mErrorStr;
        }

        // Render the string into the display
        mDisplay->clear();
        Util::Painter::writeText<Font::Font5by7>(
            mDisplay, 0U, 0U, text.c_str());
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
