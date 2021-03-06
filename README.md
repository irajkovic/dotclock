# Introduction

This repository implements a clock based on MAX7219 LED dot matrix display
connected to the Raspberry PI via SPI protocol.

The provided code represents a small framework suitable for development of
smart clock / display oriented applications. By inheriting Faces::Face base
class, a new clock face can be easily added.

While the current code has only been tested on the Raspberry PI, it should
work on any platform as long as appropriate Spi implementation is provided by
implementing the Device::Spi interface.

![Clock animation](https://raw.githubusercontent.com/wiki/irajkovic/dotclock/images/raspberry-pi-clock.gif "Clock animation")

# Usage

## Build

To build the code with CMake, run following commands:
```
mkdir build
cd build
cmake ..
make
```

Alternatively, to compile the code without the CMake, run:
```
make
```

## Running in test mode 

To run the program in the test mode:
```
touch test
./clock test
```

## Running on Raspberry PI

To run the program on a Raspberry Pi, provide the path to device file for
SPI (here assuming that the MAX7219 is connected to the first spi interface):

```
./clock /dev/spi0.0
```

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

# Integration with external programs

The clock supports the "File" face, which loads text from plain text files and
displays it in a scrolling manner. This allows for a simple integration with
external tools. Weather clock face has been provided as an example. The weather
data is first obtained from the openweathermap web api by the python script
located in the "fetch" folder. The script will dump the output to the
"tmp/weather" which is then read and displayed by the watch face
(check main.cpp). To allow for continuous update of the weather data, the
runner script could be added as a cronjob.

Before using the Weather script, API key and location needs to be set. To do
so, copy the "fetch/env_example" file to "fetch/.env" and update the API key
and location environment variables.
