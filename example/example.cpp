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
#include <fstream>
#include <signal.h>
#include "CANopen.hpp"
using namespace std;
using namespace CANopen;

Node node(4);
mutex mtx;
int sock;
bool quit = false;
uint64_t tRecv = 0, tUpdate = 0;
double t = 0.0;
double x = 0.0;
// double y = 0.0;
const double dt = 0.001;
const double a = 120.0;
const double f = 0.1;
const double w = 2.0 * M_PI * f;

void func()
{
    while (true)
    {
        can_frame canFrame;
        Frame CANopenFrame;
        if (recv(sock, &canFrame, sizeof(canFrame), 0))
        {
            mtx.lock();
            CANopenFrame.dlc = canFrame.can_dlc;
            CANopenFrame.cobId.bits.nodeId = canFrame.can_id & 0x7F;
            CANopenFrame.cobId.bits.functionCode = (canFrame.can_id & 0x780) >> 7;
            CANopenFrame.rtr = canFrame.can_id & CAN_RTR_FLAG;
            memcpy(CANopenFrame.data, canFrame.data, canFrame.can_dlc);
            auto start = chrono::steady_clock::now();
            node.receiveFrame(CANopenFrame);
            auto end = chrono::steady_clock::now();
            tRecv = chrono::duration_cast<chrono::microseconds>(end - start).count();
            mtx.unlock();
        }
    }
}

void signal_callback_handler(int signum)
{
    quit = true;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signal_callback_handler);
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

    // node.loadOD();
    Object *object = node.findObject(0x6048);
    thread listenThread(func);
    listenThread.detach();
    while (!quit)
    {
        if (mtx.try_lock())
        {
            // if (node.getNmtState() == NMTState_PreOperational)
            //     node.setNmtTransition(NMTServiceCommand_Start);
            object->setValue(1, x);
            auto start = chrono::steady_clock::now();
            node.update();
            auto end = chrono::steady_clock::now();
            tUpdate = chrono::duration_cast<chrono::microseconds>(end - start).count();
            // node.transmitPDO(0);
            mtx.unlock();
            // printf("[main] update: %ld µs, receive: %ld µs\n", tUpdate, tRecv);
        }
        t += dt;
        x = a * sin(w * t);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    // node.saveOD();
    return EXIT_SUCCESS;
}

void Node::sendFrame(Frame frame)
{
    can_frame canFrame;
    canFrame.len = frame.dlc;
    canFrame.can_id = frame.cobId.value;
    memcpy(canFrame.data, frame.data, frame.dlc);
    int size = sizeof(canFrame);
    if (send(sock, &canFrame, size, 0) != size)
    {
        perror("Write");
        exit(EXIT_FAILURE);
    }
}

void ObjectDictionnary::saveData()
{
    std::ofstream f("file.dat", std::ios::out | std::ios::binary);
    if (!f)
        return;
    f.write((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
}

void ObjectDictionnary::loadData()
{
    std::ifstream f("file.dat", std::ios::in | std::ios::binary);
    if (!f)
        return;
    f.read((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
}

uint32_t Node::getTime_us()
{
    return (uint32_t)chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}
