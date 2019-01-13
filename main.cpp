#include <memory>
#include <vector>

#include "Device/scrolling-display.hpp"
#include "Max7219/display.hpp"
#include "Max7219/spi.hpp"

#include "Faces/date.hpp"
#include "Faces/runner.hpp"
#include "Faces/text.hpp"
#include "Faces/time.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <spi-device>" << std::endl;
        return 0;
    }

    Max7219::Spi spi(argv[1], true);
    Max7219::Display display(spi, 32U);
    Device::ScrollingDisplay scrollingDisplay(&display);

    Faces::Text separator(&scrollingDisplay, " ");

    std::vector<std::unique_ptr<Faces::Face>> faces;
    faces.emplace_back(std::make_unique<Faces::Time>(&scrollingDisplay));
    faces.emplace_back(std::make_unique<Faces::Date>(&scrollingDisplay));

    Faces::Runner runner(faces, separator);
    runner.run();

    return 0;
}
