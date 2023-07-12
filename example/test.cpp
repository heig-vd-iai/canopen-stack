#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <linux/can.h>
#include "CANopen.hpp"
using namespace std;

CANopen_Node node(1);
int sock;

void func()
{
    while (true)
    {
        can_frame canFrame;
        CANopen_Frame CANopenFrame;
        if (recv(sock, &canFrame, sizeof(canFrame), 0))
        {
            CANopenFrame.dlc = canFrame.can_dlc;
            node.receiveFrame(CANopenFrame);
        }
    }
}

int main()
{
    thread t(func);
    t.join();
    return EXIT_SUCCESS;
}
