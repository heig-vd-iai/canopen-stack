#include "object_1001.hpp"
using namespace CANopen;

void Object1001::writeErrorBit(unsigned bit, bool value)
{
    ErrorRegister reg = {getValue()};
    switch (bit)
    {
    case ErrorRegisterBit_Generic:
        reg.bits.genericError = value;
        break;
    case ErrorRegisterBit_Current:
        reg.bits.current = value;
        break;
    case ErrorRegisterBit_Voltage:
        reg.bits.voltage = value;
        break;
    case ErrorRegisterBit_Temperature:
        reg.bits.temperature = value;
        break;
    case ErrorRegisterBit_Communication:
        reg.bits.communicationError = value;
        break;
    case ErrorRegisterBit_DeviceProfile:
        reg.bits.deviceProfileSpecific = value;
        break;
    case ErrorRegisterBit_Manufacturer:
        reg.bits.manufacturerSpecific = value;
        break;
    }
    setValue(0, reg.value);
}

uint8_t Object1001::getValue()
{
    return *(uint8_t *)entries[0].dataSrc;
}

void Object1001::setErrorBit(unsigned bit)
{ // Generic error shall be signaled at any error situation
    writeErrorBit(bit, true);
    writeErrorBit(ErrorRegisterBit_Generic, true);
}

void Object1001::clearErrorBit(unsigned bit)
{
    writeErrorBit(bit, false);
}

bool Object1001::isErrorfree()
{
    return getValue() == 0;
}
