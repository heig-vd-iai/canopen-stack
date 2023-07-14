#include "sdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstdio>

CANopen_SDO::CANopen_SDO(CANopen_Node &node) : node(node)
{
}

void CANopen_SDO::receiveFrame(CANopen_Frame frame)
{
    if (frame.nodeId != node.nodeId)
        return;
    SDO_CommandByte cmd;
    cmd.value = frame.data[0];
    printf("[SDO] received code 0x%02X:\nccs: %d\nn: %d\ne: %d\ns: %d\n", frame.functionCode, cmd.bits.ccs, cmd.bits.n, cmd.bits.e, cmd.bits.s);
    switch (frame.functionCode)
    {
    case FunctionCode_TSDO:
        receiveDownload(frame);
        break;
    case FunctionCode_RSDO:
        receiveUpload(frame);
        break;
    }
}

void CANopen_SDO::receiveUpload(CANopen_Frame request)
{
    SDO_CommandByte recvCommand;
    recvCommand.value = request.data[0];
    switch (recvCommand.bits.ccs)
    {
    case SDOClientCommandSpecifier_InitiatingUpload:
    {
        uint16_t index = request.data[2];
        index = index << 8 | request.data[1];
        uint8_t subIndex = request.data[3];
        printf("ccs type: initiating upload\nindex: 0x%04X\nsub-index: %d\n\n", index, subIndex);

        SDO_CommandByte sendCommand;
        sendCommand.bits.ccs = SDOClientCommandSpecifier_InitiatingUpload;
        sendCommand.bits.n = 0;
        sendCommand.bits.e = 0;
        sendCommand.bits.s = 1;

        CANopen_Frame response;
        response.functionCode = FunctionCode_TSDO;
        response.nodeId = node.nodeId;
        response.dlc = 8;
        response.data[0] = sendCommand.value;
        response.data[1] = request.data[1];
        response.data[2] = request.data[2];
        response.data[3] = request.data[3];
        response.data[4] = 0x08;
        response.data[5] = 0x00;
        response.data[6] = 0x00;
        response.data[7] = 0x00;
        node.sendFrame(response);
        break;
    }
    case SDOClientCommandSpecifier_SegmentUpload:
        printf("ccs type: segment upload\n\n");
        break;
    case SDOClientCommandSpecifier_BlockUpload:
        printf("ccs type: block upload\n\n");
        break;
    case SDOClientCommandSpecifier_AbortTransfer:
        uint16_t index = request.data[2];
        index = index << 8 | request.data[1];
        uint8_t subIndex = request.data[3];
        uint32_t error = request.data[7];
        error = error << 8 | request.data[6];
        error = error << 8 | request.data[5];
        error = error << 8 | request.data[4];
        printf("ccs type: abort transfer\nindex: 0x%04X\nsub-index: %d\nerror: 0x%08X\n\n", index, subIndex, error);
        break;
    }
}

void CANopen_SDO::receiveDownload(CANopen_Frame request)
{
}
