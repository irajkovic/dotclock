#include <cstring>
#include <memory>
#include <vector>

#include "device/display/max7219.hpp"
#include "device/spi/raspberry.hpp"
#include "util/scrolling-display.hpp"

#include "faces/date.hpp"
#include "faces/runner.hpp"
#include "faces/text.hpp"
#include "faces/time.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <spi-device|test>" << std::endl;
        return 0;
    }

    bool inTestMode = std::strcmp(argv[1], "test") == 0;

    Device::Spi::Raspberry spi(argv[1], inTestMode);
    Device::Display::Max7219 display(spi, 32U, inTestMode);
    Util::ScrollingDisplay scrollingDisplay(&display);

    Faces::Text separator(&scrollingDisplay, " ");

    std::vector<std::unique_ptr<Faces::Face>> faces;
    faces.emplace_back(std::make_unique<Faces::Time>(&scrollingDisplay));
    faces.emplace_back(std::make_unique<Faces::Date>(&scrollingDisplay));

    Faces::Runner runner(faces, separator);
    runner.run();

    return 0;
}
