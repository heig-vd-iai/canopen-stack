#include "object_1011.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes CANopen::Object1011::preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, Node &node)
{
    if (subindex > OBJECT_INDEX_COUNT)
    {
        uint32_t value = *(uint32_t *)bytes;
        if (value == X1011_LOAD_SIGNATURE)
            return node.od().restoreData(subindex) ? SDOAbortCode_CancelWrite : SDOAbortCode_AccessFailedHardwareError;
        else
            return SDOAbortCode_CannotStoreOrTransfer;
    }
    return SDOAbortCode_OK;
}
