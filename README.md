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

# Test mode

Development can also be done on a regular workstation without an actual
hardware, as the contents of the display can also be dumped to the standard
output. To enable this, pass "test" as a parameter instead of the actual
device file. Make sure that the test file also exist, as all SPI commands
byte stream will be dumped to this file.
Example of the console output in test mode is given below.

```
-----X----XXX-----------X----X--
----XX---X---X---------XX---XX--
-----X---X---X---X----X-X----X--
-----X----XXX--------X--X----X--
-----X---X---X---X---XXXXX---X--
-----X---X---X----------X----X--
----XXX---XXX-----------X---XXX-
--------------------------------
```

# Usage

To compile the code, simply run:
```
make
```
To run the program in the test mode:
```
touch test
./clock test
```

To run the program on a Raspberry Pi, provide the path to device file for
SPI (here assuming that the MAX7219 is connected to the first spi interface):
```
./clock /dev/spi0.0
```

