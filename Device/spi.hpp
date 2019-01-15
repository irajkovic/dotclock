#pragma once

#include <cinttypes>
#include <vector>

namespace Device
{

/**
 * This class defines the SPI interface.
 */
class Spi
{
    public:
    /**
     * Virtual destructor is a must for polymorphic base class.
     */
    virtual ~Spi() = default;

    /**
     * Writes buffer to the device via SPI protocol.
     *
     * @param[in] buffer The buffer to be sent.
     */
    virtual void write(const std::vector<uint8_t> &buffer) = 0;
};

} // namespace Device
