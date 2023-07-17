#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <net/if.h>
#include "CANopen.hpp"
using namespace std;

#define PRINT 0

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
#if PRINT
            auto start = chrono::steady_clock::now();
#endif
            node.receiveFrame(CANopenFrame);
#if PRINT
            auto end = chrono::steady_clock::now();
            printf("[main] message processing took %ld µs\n", chrono::duration_cast<chrono::microseconds>(end - start).count());
#endif
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
#if PRINT
        auto start = chrono::steady_clock::now();
#endif
        if (mtx.try_lock())
        {
            node.update();
            mtx.unlock();
        }
#if PRINT
        auto end = chrono::steady_clock::now();
        printf("[main] update took %ld µs\n", chrono::duration_cast<chrono::microseconds>(end - start).count());
#endif
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    t.join();
    return EXIT_SUCCESS;
}
