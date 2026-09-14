#pragma once

#include <cstdint>
#include <string>

#include "hal/persistence.hpp"

namespace CANopen {

/**
 * Stores each parameter group as `od-group<N>.dat` and the dictionary
 * signature as `od-signature.dat`, eight bytes per object in dictionary
 * order, the same image C2000Persistence writes to flash.
 */
class FilePersistence : public Persistence {
   public:
    explicit FilePersistence(const char *directory = ".");

    void init() override {}
    bool saveGroup(uint8_t parameterGroup) override;
    bool loadGroup(uint8_t parameterGroup) override;
    bool saveSignature(uint64_t signature) override;
    bool loadSignature(uint64_t &signature) override;

   private:
    std::string groupPath(uint8_t parameterGroup) const;
    std::string signaturePath() const;

    std::string directory;
};
}  // namespace CANopen
