#pragma once

#include <cinttypes>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace Max7219
{

/**
 * This class allows basic, single directional SPI communication to the MAX7219
 * driver circuit, used to control 7segment and dot matrix displays.
 */
class Spi
{
    public:
    /**
     * Constructs a new SPI interface object.
     *
     * @param[in] spiDevPath Path to SPI device, typically /dev/spi*.*, or
     *                       path to a regular file when debugging.
     * @param[in] fakeSpi    True if path points to a regular file, in which
     *                       case all outgoing communication is dumped to
     *                       the file.
     */
    Spi(const std::string &spiDevPath, bool fakeSpi = false)
        : mSpiDevPath(spiDevPath), mFakeDevice(fakeSpi)
    {
        mDevice = ::open(mSpiDevPath.c_str(), O_RDWR);

        if (mDevice < 0) {
            throw std::invalid_argument("Can't open SPI device");
        }

        if (mFakeDevice == false) {
            setupDevice();
        }
    }

    /**
     * Closes the device file.
     */
    ~Spi()
    {
        ::close(mDevice);
    }

    /**
     * Sets the SPI device. The settings are currently fixed and only
     * intended to be working with MAX7219 led driver.
     */
    void setupDevice()
    {
        int ret         = 0;
        int mode        = 0;
        int bitsPerWord = 8;
        int speed       = 500000;

        ret = ioctl(mDevice, SPI_IOC_WR_MODE32, &mode);
        if (ret == -1) {
            throw std::domain_error("can't set spi mode");
        }

        ret = ioctl(mDevice, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord);
        if (ret == -1) {
            throw std::domain_error("can't set bits per word");
        }

        ret = ioctl(mDevice, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if (ret == -1) {
            throw std::domain_error("can't set max speed hz");
        }
    }

    /**
     * Writes buffer to the device via SPI protocol.
     *
     * @param[in] buffer The buffer to be sent.
     */
    void write(const std::vector<uint8_t> &buffer)
    {
        /*
         * The alternative to writing to the device is using ioctl()
         * with the SPI_IOC_MESSAGE macro. This gives more control, but
         * in this case it is not needed, and using write() allows
         * regular files to be used instead of spi device file,
         * simplifying SPI protocol debugging (in this case, fakeSpi
         * must also be set to true).
         * For more info on the alternative approach, look for
         * "torvalds spi dev test".
         */
        int ret = ::write(mDevice, buffer.data(), buffer.size());
        if (ret < 1) {
            throw std::domain_error("can't send spi message");
        }
    }

    private:
    /** The path of the SPI device, typically /dev/spi*.* */
    std::string mSpiDevPath;

    /** The SPI device file descriptor. */
    int mDevice;

    /**
     * True to skip any ioctl() calls and only use system calls compatible
     * with the regular files.
     */
    bool mFakeDevice = false;
};

} /* namespace Max7219 */
