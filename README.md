# Introduction

This repository implements a clock based on MAX7219 LED dot matrix display
connected to the Raspberry PI via SPI protocol.

The provided code represents a small framework suitable for development of
smart clock / display oriented applications. By inheriting Faces::Face base
class, a new clock face can be easily added.

Currently, only date, time and text faces are provided, but more faces such
as weather forecast are planned.

While the current code has only been tested on the Raspberry PI, it should
work on any platform as long as appropriate Spi implementation is provided by
implementing the Device::Spi interface.

Development can be done without an actual hardware as display output can also
be dumped to the standard output. The Max7219::Spi is implemented so that
plain files can be used instead of actual /dev/spi*.*, allowing for the
development on a regular (Linux) workstation.

# Usage

To compile the code, simply run:
```
make
```

Then run the program providing path to device file for SPI
(here assuming that the MAX7219 is connected to the first spi interface):
```
./clock /dev/spi0.0
```

