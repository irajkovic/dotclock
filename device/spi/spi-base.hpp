#pragma once

#include <cinttypes>
#include <vector>

namespace Device
{

namespace Spi
{

/**
 * This class defines the SPI interface.
 */
class SpiBase
{
    public:
    /**
     * Virtual destructor is a must for polymorphic base class.
     */
    virtual ~SpiBase() = default;

    /**
     * Writes buffer to the device via SPI protocol.
     *
     * @param[in] buffer The buffer to be sent.
     */
    virtual void write(const std::vector<uint8_t> &buffer) = 0;
};

} // namespace Spi

} // namespace Device
