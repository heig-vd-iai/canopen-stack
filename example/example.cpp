#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstring>
#include <cmath>
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

uint32_t CANopen_Node::getTime_us()
{
    return (uint32_t)chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
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

    double t = 0.0;
    double x = 0.0;
    // double y = 0.0;
    const double dt = 0.001;
    const double a = 120.0;
    const double f = 0.1;
    const double w = 2.0 * M_PI * f;
    Object *object = node.od.findObject(0x6048);
    uint32_t errorCode = 0;
    thread listenThread(func);
    while (true)
    {
#if PRINT
        auto start = chrono::steady_clock::now();
#endif
        if (mtx.try_lock())
        {
            object->writeBytes(1, (uint8_t *)&x, sizeof(x), &errorCode, node);
            // object->readBytes(1, (uint8_t *)&y, sizeof(y), 0, &errorCode);
            node.update();
            mtx.unlock();
        }
        t += dt;
        x = a * sin(w * t);
        // printf("[main] x = %lf, y = %lf\n", x, y);
#if PRINT
        auto end = chrono::steady_clock::now();
        printf("[main] update took %ld µs\n", chrono::duration_cast<chrono::microseconds>(end - start).count());
#endif
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    listenThread.join();
    return EXIT_SUCCESS;
}
