#include <cstdint>

struct CRCCheck {
    static constexpr uint16_t polynomial = 0x1021; // CRC-16-CCITT polynomial

    // Add a byte to the CRC calculation
    // This uses the CRC-16-CCITT polynomial
    void byte(uint8_t data) {
        crc_ ^= (uint16_t)data << 8;
        for (int i = 0; i < 8; ++i) {
            if (crc_ & 0x8000) {
                crc_ = (crc_ << 1) ^ polynomial;
            } else {
                crc_ <<= 1;
            }
        }
        crc_ &= 0xFFFF; // Ensure the result stays 16 bits
    }

    // Get the current CRC value
    uint16_t result() const {
        return crc_;
    }

    // Reset the CRC
    void reset() {
        crc_ = 0x0000;
    }

private:
    uint16_t crc_ = 0x0000; // Initial CRC value
};
