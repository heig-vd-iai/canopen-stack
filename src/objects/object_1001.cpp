#include "object_1001.hpp"
using namespace CANopen;

uint8_t Object1001::getValue()
{
    return *(uint8_t *)entries[0].dataSrc;
}

void Object1001::setErrorBit(unsigned bit)
{ // Generic error shall be signaled at any error situation
    ErrorRegister reg = {getValue()};
    reg.bits.genericError = 1;
    switch (bit)
    {
    case ErrorRegisterBit_Current:
        reg.bits.current = 1;
        break;
    case ErrorRegisterBit_Voltage:
        reg.bits.voltage = 1;
        break;
    case ErrorRegisterBit_Temperature:
        reg.bits.temperature = 1;
        break;
    case ErrorRegisterBit_Communication:
        reg.bits.communicationError = 1;
        break;
    case ErrorRegisterBit_DeviceProfile:
        reg.bits.deviceProfileSpecific = 1;
        break;
    case ErrorRegisterBit_Manufacturer:
        reg.bits.manufacturerSpecific = 1;
        break;
    }
    setValue(0, reg.value);
}

void Object1001::clearErrorBit(unsigned bit)
{
    ErrorRegister reg = {getValue()};
    switch (bit)
    {
    case ErrorRegisterBit_Generic:
        // Only clear generic error bit is all other bits are clear
        if (reg.value == 0b00000001)
            reg.bits.genericError = 0;
        break;
    case ErrorRegisterBit_Current:
        reg.bits.current = 0;
        break;
    case ErrorRegisterBit_Voltage:
        reg.bits.voltage = 0;
        break;
    case ErrorRegisterBit_Temperature:
        reg.bits.temperature = 0;
        break;
    case ErrorRegisterBit_Communication:
        reg.bits.communicationError = 0;
        break;
    case ErrorRegisterBit_DeviceProfile:
        reg.bits.deviceProfileSpecific = 0;
        break;
    case ErrorRegisterBit_Manufacturer:
        reg.bits.manufacturerSpecific = 0;
        break;
    }
    setValue(0, reg.value);
}

bool Object1001::isErrorfree()
{
    return getValue() == 0;
}
