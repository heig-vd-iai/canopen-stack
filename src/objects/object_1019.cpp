/**
 * Contains the definitions of the "Synchronous counter overflow value" Object1019 class.
 */
#include "object_1019.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes CANopen::Object1019::preWriteBytes(uint8_t /*subindex*/, uint8_t *bytes, uint32_t /*size*/, Node & /*node*/)
{
    uint8_t currentValue, newValue;
    getValue(0, &currentValue);
    if (currentValue != X1019_NO_COUNTER)
        return SDOAbortCode_CannotStoreOrTransfer_DeviceState;
    newValue = bytes[0];
    if (!(newValue == X1019_NO_COUNTER || (X1019_MIN_COUNTER <= newValue && newValue <= X1019_MAX_COUNTER)))
        return SDOAbortCode_InvalidDownloadParameterValue;
    return SDOAbortCode_OK;
}
