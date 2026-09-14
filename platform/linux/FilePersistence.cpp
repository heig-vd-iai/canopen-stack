#include "FilePersistence.hpp"

#include <fstream>

#include "node.hpp"
#include "od/parameterGroup.hpp"
#include "od_common.hpp"
#include "od_lookup.hpp"

using namespace CANopen;

FilePersistence::FilePersistence(const char *directory)
    : directory(directory) {}

std::string FilePersistence::groupPath(uint8_t parameterGroup) const {
    return directory + "/od-group" + std::to_string(parameterGroup) + ".dat";
}

std::string FilePersistence::signaturePath() const {
    return directory + "/od-signature.dat";
}

bool FilePersistence::saveGroup(uint8_t parameterGroup) {
    std::ofstream file(groupPath(parameterGroup),
                       std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file) return false;
    ObjectDictionnary &od = node.od();
    for (int32_t id = 0; id < static_cast<int32_t>(od.length); id++) {
        if (!inParameterGroup(CANopenOD::objectIndexTable[id].first,
                              parameterGroup))
            continue;
        Data value;
        value.u64 = 0;
        SDOAbortCodes abortCode;
        readDataWait(value, id, abortCode);
        file.write(reinterpret_cast<const char *>(&value.u64),
                   sizeof(value.u64));
    }
    return static_cast<bool>(file);
}

bool FilePersistence::loadGroup(uint8_t parameterGroup) {
    std::ifstream file(groupPath(parameterGroup),
                       std::ios::in | std::ios::binary);
    if (!file) return false;
    ObjectDictionnary &od = node.od();
    for (int32_t id = 0; id < static_cast<int32_t>(od.length); id++) {
        if (!inParameterGroup(CANopenOD::objectIndexTable[id].first,
                              parameterGroup))
            continue;
        Data value;
        value.u64 = 0;
        if (!file.read(reinterpret_cast<char *>(&value.u64), sizeof(value.u64)))
            return false;
        SDOAbortCodes abortCode;
        writeDataWait(value, id, abortCode);
    }
    return true;
}

bool FilePersistence::saveSignature(uint64_t signature) {
    std::ofstream file(signaturePath(),
                       std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file) return false;
    file.write(reinterpret_cast<const char *>(&signature), sizeof(signature));
    return static_cast<bool>(file);
}

bool FilePersistence::loadSignature(uint64_t &signature) {
    std::ifstream file(signaturePath(), std::ios::in | std::ios::binary);
    if (!file) return false;
    return static_cast<bool>(
        file.read(reinterpret_cast<char *>(&signature), sizeof(signature)));
}
