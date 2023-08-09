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
#include <bitset>
#include <iostream>
#include "CANopen.hpp"
using namespace std;
using namespace CANopen;
#define ID_MIN 1
#define ID_MAX 127
// #define INTERACTIVE

Node *nodePtr;
mutex mtx;
int sock;
bool quit = false;
uint64_t tRecv = 0, tUpdate = 0;

void listenFunc()
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
            nodePtr->receiveFrame(CANopenFrame);
            auto end = chrono::steady_clock::now();
            tRecv = chrono::duration_cast<chrono::microseconds>(end - start).count();
            mtx.unlock();
        }
    }
}

void updateFunc()
{
    double t = 0.0;
    double x = 0.0;
    const double dt = 0.001;
    const double a = 120.0;
    const double f = 0.1;
    const double w = 2.0 * M_PI * f;
    while (!quit)
    {
        if (mtx.try_lock())
        {
            nodePtr->at(OD_OBJECT_6048)->setValue(1, x);
            auto start = chrono::steady_clock::now();
            nodePtr->update();
            auto end = chrono::steady_clock::now();
            tUpdate = chrono::duration_cast<chrono::microseconds>(end - start).count();
            // nodePtr->transmitPDO(0);
            mtx.unlock();
            // printf("[main] update: %ld µs, receive: %ld µs\n", tUpdate, tRecv);
        }
        t += dt;
        x = a * sin(w * t);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void signal_callback_handler(int signum)
{
    quit = true;
}

int main(int argc, char *argv[])
{
    unsigned nodeID;
    int ifindex;
    signal(SIGINT, signal_callback_handler);
    try
    {
        nodeID = stoi(argv[2]);
        if ((ifindex = if_nametoindex(argv[1])) == 0)
        {
            cout << "Unknown interface \"" << argv[1] << "\"" << endl;
            throw exception();
        }
    }
    catch (...)
    {
        cout << "Usage: ./app <CAN interface> <node ID>" << endl;
        return EXIT_FAILURE;
    }
    if (nodeID < ID_MIN || ID_MAX < nodeID)
    {
        cout << "Node ID must be in range " << ID_MIN << " to " << ID_MAX << endl;
        return EXIT_FAILURE;
    }

    if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return EXIT_FAILURE;
    }
    sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifindex;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return EXIT_FAILURE;
    }

    cout << "Starting node with ID " << nodeID << " on interface " << argv[1] << endl;
    Node node(nodeID);
    nodePtr = &node;
    // node.loadOD();
    thread listenThread(listenFunc);
    listenThread.detach();
    thread updateThread(updateFunc);
#ifdef INTERACTIVE
    int choice = 0;
    do
    {
        cout << "===== CANopen example =====\n";
        cout << "0: Quit\n";
        cout << "1: Emit generic error\n11: Clear generic error\n";
        cout << "2: Emit current error\n22: Clear current error\n";
        cout << "3: Emit voltage error\n33: Clear voltage error\n";
        cout << "4: Emit communication error\n44: Clear communication error\n";
        cout << ">";
        cin >> choice;
        switch (choice)
        {
        case 0:
            quit = true;
            break;
        case 1:
            node.emcy.raiseError(EMCYErrorCode_Generic);
            break;
        case 11:
            node.emcy.clearErrorBit(ErrorRegisterBit_Generic);
            break;
        case 2:
            node.emcy.raiseError(EMCYErrorCode_Current);
            break;
        case 22:
            node.emcy.clearErrorBit(ErrorRegisterBit_Current);
            break;
        case 3:
            node.emcy.raiseError(EMCYErrorCode_Voltage);
            break;
        case 33:
            node.emcy.clearErrorBit(ErrorRegisterBit_Voltage);
            break;
        case 4:
            node.emcy.raiseError(EMCYErrorCode_Communication_CANErrorPassive);
            break;
        case 44:
            node.emcy.clearErrorBit(ErrorRegisterBit_Communication);
            break;
        }
    } while (choice != 0);
    quit = true;
#endif
    updateThread.join();
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

bool ObjectDictionnary::saveData(uint8_t parameterGroup)
{
    std::ofstream f("file.dat", std::ios::out | std::ios::binary);
    if (!f)
        return false;
    f.write((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    return true;
}

bool ObjectDictionnary::loadData(uint8_t parameterGroup)
{
    std::ifstream f("file.dat", std::ios::in | std::ios::binary);
    if (!f)
        return false;
    f.read((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    return true;
}

bool ObjectDictionnary::restoreData(uint8_t parameterGroup)
{
    return true;
}

uint32_t Node::getTime_us()
{
    return (uint32_t)chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}
