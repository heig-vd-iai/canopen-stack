#pragma once

#include <cstdint>

#include "hal/persistence.hpp"

namespace CANopen {

/**
 * Object dictionary images in CM flash bank 0 through the F021 Flash API.
 * One sector per parameter group, eight bytes per object in dictionary
 * order, plus a sector holding the dictionary signature.
 */
class C2000Persistence : public Persistence {
   public:
    void init() override;
    bool saveGroup(uint8_t parameterGroup) override;
    bool loadGroup(uint8_t parameterGroup) override;
    bool saveSignature(uint64_t signature) override;
    bool loadSignature(uint64_t &signature) override;

   private:
    struct Sector {
        uint8_t parameterGroup;
        uint32_t origin;
        uint32_t length;
    };

    static const Sector sectors[];
    static const Sector signatureSector;

    static const Sector *sectorOf(uint8_t parameterGroup);
    static bool waitFsmReady();
    static bool eraseSector(const Sector &sector);
    static bool isBlank(uint32_t address, uint32_t words);
    static bool program(uint32_t address, const uint64_t &value);
};
}  // namespace CANopen
