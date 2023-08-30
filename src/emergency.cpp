/******************************************************************************
 * [Filename]:      emergency.cpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the definitions of the EMCY class.
 *****************************************************************************/
#include "emergency.hpp"
#include "node.hpp"
#include "objects/object_1001.hpp"
#include "objects/object_1003.hpp"
#include "frame.hpp"
using namespace CANopen;

EMCY::EMCY(Node &node) : node(node)
{
    errorRegisterObject = (Object1001 *)node._od.at(OD_OBJECT_1001);
}

void EMCY::enable() { enabled = true; }

void EMCY::disable() { enabled = false; }

void CANopen::EMCY::sendError(uint16_t errorCode, uint32_t manufacturerCode)
{
    EmergencyFrame frame(node.nodeId, errorCode, errorRegisterObject->getValue(), manufacturerCode);
    node.sendFrame(frame);
}

void EMCY::raiseError(uint16_t errorCode, uint16_t manufacturerCode)
{
    if (!enabled)
        return;
    uint8_t behaviour = X1029_BEHAVIOUR_PREOP;
    switch ((EMCYErrorCodes)errorCode)
    {
    case EMCYErrorCode_Generic:
    case EMCYErrorCode_DeviceHardware:
    case EMCYErrorCode_Software:
    case EMCYErrorCode_Software_Internal:
    case EMCYErrorCode_Software_User:
    case EMCYErrorCode_Software_Dataset:
    case EMCYErrorCode_Modules:
    case EMCYErrorCode_Monitoring:
    case EMCYErrorCode_Protocol:
    case EMCYErrorCode_Protocol_PDOLengthError:
    case EMCYErrorCode_Protocol_PDOLengthExceeded:
    case EMCYErrorCode_Protocol_DAMMPDODestinationNotAvailable:
    case EMCYErrorCode_Protocol_SyncDataLength:
    case EMCYErrorCode_Protocol_RPDOTimeout:
    case EMCYErrorCode_ExternalError:
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
    case EMCYErrorCode_Communication:
    case EMCYErrorCode_Communication_CANOverrun:
    case EMCYErrorCode_Communication_CANErrorPassive:
    case EMCYErrorCode_Communication_HeartbeatError:
    case EMCYErrorCode_Communication_RecoveredBusOFF:
    case EMCYErrorCode_Communication_CANIDCollision:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Communication);
#ifdef OD_OBJECT_1029
        node._od.at(OD_OBJECT_1029)->getValue(X1029_SUB_COMMUNICATION, &behaviour);
#endif
        break;
    case EMCYErrorCode_DeviceSpecific:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_DeviceProfile);
        break;
    case EMCYErrorCode_AdditionalFunctions:
        errorRegisterObject->setErrorBit(ErrorRegisterBit_Manufacturer);
        break;
    default:
        break;
    }
#ifdef OD_OBJECT_1003
    ((Object1003 *)node._od.at(OD_OBJECT_1003))->pushError(errorCode, manufacturerCode);
#endif
    sendError(errorCode, manufacturerCode);
    NMTServiceCommands command = NMTServiceCommand_None;
    switch (behaviour)
    {
    default:
    case X1029_BEHAVIOUR_PREOP:
        if (node._nmt.getState() == NMTState_Operational)
            command = NMTServiceCommand_EnterPreOperational;
        break;
    case X1029_BEHAVIOUR_NONE:
        command = NMTServiceCommand_None;
        break;
    case X1029_BEHAVIOUR_STOPPED:
        command = NMTServiceCommand_Stop;
        break;
    }
    node._nmt.setTransition(command);
}

void EMCY::clearErrorBit(unsigned bit)
{
    bool tmp = errorRegisterObject->isErrorfree();
    errorRegisterObject->clearErrorBit(bit);
    if (errorRegisterObject->isErrorfree() && !tmp)
        sendError(EMCYErrorCode_Reset, 0);
}

uint8_t EMCY::getErrorRegister()
{
    return errorRegisterObject->getValue();
}

void EMCY::clearHistory()
{
#ifdef OD_OBJECT_1003
    ((Object1003 *)node._od.at(OD_OBJECT_1003))->clearErrors();
#endif
}

void CANopen::EMCY::reset()
{
    errorRegisterObject->reset();
}
