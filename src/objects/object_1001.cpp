#include "object_1001.hpp"
using namespace CANopen;

void Object1001::writeErrorBit(unsigned bit, bool value)
{
    ErrorRegister reg = {getValue()};
    switch (bit)
    {
    case X1001_BIT_CURRENT:
        reg.bits.current = value;
        break;
    case X1001_BIT_VOLTAGE:
        reg.bits.voltage = value;
        break;
    case X1001_BIT_TEMPERATURE:
        reg.bits.temperature = value;
        break;
    case X1001_BIT_COMMUNICATION:
        reg.bits.communicationError = value;
        break;
    case X1001_BIT_DEVICEPROFILE:
        reg.bits.deviceProfileSpecific = value;
        break;
    case X1001_BIT_MANUFACTURER:
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
{
    writeErrorBit(bit, true);
}

void Object1001::clearErrorBit(unsigned bit)
{
    writeErrorBit(bit, false);
}

bool Object1001::isErrorfree()
{
    return getValue() == 0;
}
