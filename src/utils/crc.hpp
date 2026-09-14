#pragma once

#include <cstdint>

namespace CANopen {

/**
 * CRC-16-CCITT as required by CiA301:2011 §7.2.4.3.16: polynomial
 * x^16 + x^12 + x^5 + 1, initial value 0.
 */
class Crc16Ccitt {
   public:
    void reset() { value = 0; }

    void feed(uint8_t byte) {
        value = static_cast<uint16_t>(value ^ static_cast<uint16_t>(byte << 8));
        for (unsigned i = 0; i < 8; i++) {
            const bool carry = (value & 0x8000u) != 0;
            value = static_cast<uint16_t>(value << 1);
            if (carry) value = static_cast<uint16_t>(value ^ POLYNOMIAL);
        }
    }

    void feed(const uint8_t *bytes, uint32_t length) {
        for (uint32_t i = 0; i < length; i++) feed(bytes[i]);
    }

    uint16_t result() const { return value; }

   private:
    static constexpr uint16_t POLYNOMIAL = 0x1021;
    uint16_t value = 0;
};

}  // namespace CANopen
