#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <linux/can.h>
#include <net/if.h>
#include "CANopen.hpp"
using namespace std;

CANopen_Node node(1);
int sock;

void func()
{
    while (true)
    {
        can_frame canFrame;
        // CANopen_Frame CANopenFrame;
        if (recv(sock, &canFrame, sizeof(canFrame), 0))
        {
            // CANopenFrame.dlc = canFrame.can_dlc;
            printf("Received CAN frame\nID: %d\nData [%d]: ", canFrame.can_id, canFrame.can_dlc);
            for (int i = 0; i < canFrame.can_dlc; i++)
                printf("%X ", canFrame.data[i]);
            printf("\n\n");
            // node.receiveFrame(CANopenFrame);
        }
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
    t.join();
    return EXIT_SUCCESS;
}
