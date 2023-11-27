/**
 * Contains the definitions of the "Store parameters" Object1010 class.
 */
#include "object_1010.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes CANopen::Object1010::preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t /*sizeBytes*/, Node &node)
{
    if (subindex > OBJECT_INDEX_COUNT)
    {
        uint32_t value = *(uint32_t *)bytes;
        if (value == X1010_SAVE_SIGNATURE)
            return node.od().saveData(subindex) ? SDOAbortCode_CancelWrite : SDOAbortCode_AccessFailedHardwareError;
        else
            return SDOAbortCode_CannotStoreOrTransfer;
    }
    return SDOAbortCode_OK;
}
