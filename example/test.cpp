#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <net/if.h>
#include "CANopen.hpp"
using namespace std;

CANopen_Node node(4);
mutex mtx;
int sock;

void func()
{
    while (true)
    {
        can_frame canFrame;
        CANopen_Frame CANopenFrame;
        if (recv(sock, &canFrame, sizeof(canFrame), 0))
        {
            mtx.lock();
            CANopenFrame.dlc = canFrame.can_dlc;
            CANopenFrame.nodeId = canFrame.can_id & 0x7F;
            CANopenFrame.functionCode = (canFrame.can_id & 0x780) >> 7;
            memcpy(CANopenFrame.data, canFrame.data, canFrame.can_dlc);
#if 0
            printf("Received CAN frame\nID: %d\nData [%d]: ", canFrame.can_id, canFrame.can_dlc);
            for (int i = 0; i < canFrame.can_dlc; i++)
                printf("%02X ", canFrame.data[i]);
            printf("\nCANopen function code (0x%X) and node ID (%d)\n\n", CANopenFrame.functionCode, CANopenFrame.nodeId);
#endif
            node.receiveFrame(CANopenFrame);
            mtx.unlock();
        }
    }
}

void CANopen_Node::sendFrame(CANopen_Frame frame)
{
    can_frame canFrame;
    canFrame.can_dlc = frame.dlc;
    canFrame.can_id = (uint32_t)frame.functionCode << 7 | frame.nodeId;
    memcpy(canFrame.data, frame.data, frame.dlc);
    int size = sizeof(canFrame);
    if (send(sock, &canFrame, size, 0) != size)
    {
        perror("Write");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return EXIT_FAILURE;
    }
    sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = if_nametoindex("vcan0");
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return EXIT_FAILURE;
    }

    thread t(func);
    while (true)
    {
        if (mtx.try_lock())
        {
            node.update();
            mtx.unlock();
        }
    }

    t.join();
    return EXIT_SUCCESS;
}
