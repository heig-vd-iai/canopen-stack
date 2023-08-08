#include "emergency.hpp"
#include "node.hpp"
#include "objects/object_1001.hpp"
#include "objects/object_1003.hpp"
#include "frame.hpp"
#include <cstdlib>
using namespace CANopen;

EMCY::EMCY(Node &node) : node(node)
{
    errorRegisterObject = (Object1001 *)node.at(OD_OBJECT_1001);
}

void CANopen::EMCY::sendError(uint16_t errorCode, uint32_t manufacturerCode)
{
    Frame frame;
    frame.cobId.bits.functionCode = FunctionCode_EMCY;
    frame.cobId.bits.nodeId = node.nodeId;
    frame.dlc = 8;
    *(uint16_t *)frame.data = errorCode;
    frame.data[2] = errorRegisterObject->getValue();
    *(uint32_t *)(frame.data + 3) = manufacturerCode;
    node.sendFrame(frame);
}

void EMCY::enable() { enabled = true; }

void EMCY::disable() { enabled = false; }

void EMCY::receiveFrame(Frame frame)
{
    if (!enabled)
        return;
}

void EMCY::transmitError(uint16_t errorCode, uint16_t manufacturerCode)
{
    if (!enabled)
        return;
    switch (errorCode & 0xFF00)
    {
    case EMCYErrorCode_Generic:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Generic);
        break;
    case EMCYErrorCode_Current:
    case EMCYErrorCode_Current_InputSide:
    case EMCYErrorCode_Current_InsideDevice:
    case EMCYErrorCode_Current_OutputSide:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Current);
        break;
    case EMCYErrorCode_Voltage:
    case EMCYErrorCode_Voltage_Main:
    case EMCYErrorCode_Voltage_InsideDevice:
    case EMCYErrorCode_Voltage_Output:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Voltage);
        break;
    case EMCYErrorCode_Temperature:
    case EMCYErrorCode_Temperature_Ambient:
    case EMCYErrorCode_Temperature_Device:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Temperature);
        break;
    case EMCYErrorCode_Communication_CANOverrun:
    case EMCYErrorCode_Communication_CANErrorPassive:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Communication);
        break;
    case EMCYErrorCode_DeviceSpecific:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_DeviceProfile);
        break;
    case EMCYErrorCode_AdditionalFunctions:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Manufacturer);
        break;
    }
#ifdef OD_OBJECT_1003
    ((Object1003 *)node.at(OD_OBJECT_1003))->pushError(errorCode, manufacturerCode);
#endif
    sendError(errorCode, manufacturerCode);
}

void EMCY::clearErrorBit(unsigned bit)
{
    bool tmp = errorRegisterObject->isErrorfree();
    errorRegisterObject->clearErrorBit(bit);
    if (errorRegisterObject->isErrorfree() && !tmp)
        sendError(EMCYErrorCode_Reset, 0);
}

void EMCY::clearHistory()
{
#ifdef OD_OBJECT_1003
    if (enabled)
        ((Object1003 *)node.at(OD_OBJECT_1003))->clearErrors();
#endif
}
