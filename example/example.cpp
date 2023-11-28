#include "CANopen.hpp"
#include <bitset>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <linux/can.h>
#include <mutex>
#include <net/if.h>
#include <pthread.h>
#include <random>
#include <signal.h>
#include <sys/socket.h>
#include <thread>
using namespace CANopen;
using namespace std;
#define ID_MAX 127
#define ID_MIN 1
#define FILENAME "od.dat"
// #define INTERACTIVE // uncomment to enable interactive mode

struct ErrorCodeInfo
{
    uint16_t code;
    const char *description;
};

const ErrorCodeInfo errorInfo[] = {
    {0x0000, "Error reset or no error"},
    {0x1000, "Generic error"},
    {0x2000, "Current - generic error"},
    {0x2100, "Current, CANopen device input side - generic"},
    {0x2200, "Current inside the CANopen device - generic"},
    {0x2300, "Current, CANopen device output side - generic"},
    {0x3000, "Voltage - generic error"},
    {0x3100, "Mains voltage - generic"},
    {0x3200, "Voltage inside the CANopen device - generic"},
    {0x3300, "Output voltage - generic"},
    {0x4000, "Temperature - generic error"},
    {0x4100, "Ambient temperature - generic"},
    {0x4200, "Device temperature - generic"},
    {0x5000, "CANopen device hardware - generic error"},
    {0x6000, "CANopen device software - generic error"},
    {0x6100, "Internal software - generic"},
    {0x6200, "User software - generic"},
    {0x6300, "Data set - generic"},
    {0x7000, "Additional modules - generic error"},
    {0x8000, "Monitoring - generic error"},
    {0x8100, "Communication - generic"},
    {0x8110, "CAN overrun (objects lost)"},
    {0x8120, "CAN in error passive mode"},
    {0x8130, "Life guard error or heartbeat error"},
    {0x8140, "Recovered from bus off"},
    {0x8150, "CAN-ID collision"},
    {0x8200, "Protocol error - generic"},
    {0x8210, "PDO not processed due to length error"},
    {0x8220, "PDO length exceeded"},
    {0x8230, "DAM MPDO not processed, destination object not available"},
    {0x8240, "Unexpected SYNC data length"},
    {0x8250, "RPDO timeout"},
    {0x9000, "External error - generic error"},
    {0xF000, "Additional functions - generic error"},
    {0xFF00, "Device specific - generic error"}};
const int errorInfoLength = sizeof(errorInfo) / sizeof(errorInfo[0]);
bool quit = false;
int sock = -1;

#ifdef INTERACTIVE
void clear()
{
    cin.clear();
    cin.ignore(1024, '\n');
}
#endif

void listenFunc(Node &node, mutex &mtx)
{
    while (true)
    {
        can_frame canFrame;
        if (recv(sock, &canFrame, sizeof(canFrame), 0))
        {
            mtx.lock();
            Frame CANopenFrame = Frame::fromCobId(canFrame.can_id);
            CANopenFrame.dlc = canFrame.can_dlc;
            CANopenFrame.rtr = canFrame.can_id & CAN_RTR_FLAG;
            memcpy(CANopenFrame.data, canFrame.data, canFrame.can_dlc);
            node.receiveFrame(CANopenFrame);
            mtx.unlock();
        }
    }
}

void updateFunc(Node &node, mutex &mtx)
{
    default_random_engine engine;
    uniform_real_distribution<double> unif(100.0, 200.0);
    while (!quit)
    {
        if (mtx.try_lock())
        {
            node.od()[OD_OBJECT_6048]->setValue(1, unif(engine));
            node.update();
            mtx.unlock();
        }
    }
}

void onWrite(Object &object, unsigned subindex)
{
    printf("Object %X at subindex %d was written to\n", object.index, subindex);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, [](int /*signum*/)
           { quit = true; });
    int ifindex;
    if (argc < 2)
    {
        cout << "Usage: ./app <CAN interface>" << endl;
        return EXIT_FAILURE;
    }
    if ((ifindex = if_nametoindex(argv[1])) == 0)
    {
        cout << "Unknown interface \"" << argv[1] << "\"" << endl;
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

    Node node;
    cout << "Starting node with ID " << (int)node.nodeId << " on interface " << argv[1] << endl;
#ifndef INTERACTIVE
    node.pdo().onTimeout([](unsigned index)
                         { cout << "Timeout occured on RPDO" << index << endl; });
    node.pdo().onReceive([](unsigned index)
                         { cout << "Received RPDO" << index << endl; });
    for (int i = 0; i < node.od().length; i++)
        node.od()[i]->onWrite(onWrite);
#endif
    node.od().loadData(0);
    node.init();
    mutex mtx;
    thread listenThread(listenFunc, ref(node), ref(mtx));
    listenThread.detach();
    thread updateThread(updateFunc, ref(node), ref(mtx));
#ifdef INTERACTIVE
    int choice = 0, subChoice = 0;
    do
    {
        cout << "===== CANopen example =====\n";
        cout << "0: Quit\n";
        cout << "1: Save OD\n";
        cout << "2: Load OD\n";
        cout << "3: Restore OD\n";
        cout << "4: Raise error\n";
        cout << "5: Error register\n";
        cout << "6: Clear error history\n";
        cout << "7: Transmit PDO\n";
        cout << "> ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            node.od().saveData(0);
            break;
        case 2:
            node.od().loadData(0);
            break;
        case 3:
            node.od().restoreData(0);
            break;
        case 4:
        {
            cout << "===== Error codes =====\n";
            cout << "0: Return\n";
            for (int i = 0; i < errorInfoLength; i++)
                cout << i + 1 << ": " << errorInfo[i].description << '\n';
            cout << "> ";
            cin >> subChoice;
            if (1 <= subChoice && subChoice <= errorInfoLength)
                node.emcy().raiseError(errorInfo[subChoice - 1].code);
            clear();
            break;
        }
        case 5:
        {
            bitset<8> reg{node.emcy().getErrorRegister()};
            cout << "===== Error register =====\n";
            cout << "0: Return\n";
            cout << "1: (" << reg[0] << ") Clear generic error\n";
            cout << "2: (" << reg[1] << ") Clear current error\n";
            cout << "3: (" << reg[2] << ") Clear voltage error\n";
            cout << "4: (" << reg[3] << ") Clear temperature error\n";
            cout << "5: (" << reg[4] << ") Clear communication error\n";
            cout << "6: (" << reg[5] << ") Clear device profile specific error\n";
            cout << "7: Reserved\n";
            cout << "8: (" << reg[7] << ") Clear manufacturer error\n";
            cout << "> ";
            cin >> subChoice;
            if (1 <= subChoice && subChoice <= 8)
                node.emcy().clearErrorBit(subChoice - 1);
            clear();
            break;
        }
        case 6:
            node.emcy().clearHistory();
            break;
        case 7:
        {
            cout << "===== TPDOs =====\n";
            cout << "0: Return\n";
            for (int i = 0; i < OD_TPDO_COUNT; i++)
                cout << i + 1 << ": send TPDO" << i + 1 << '\n';
            cout << "> ";
            cin >> subChoice;
            if (1 <= subChoice && subChoice <= OD_TPDO_COUNT)
                node.pdo().transmitTPDO(subChoice - 1);
            clear();
            break;
        }
        }
    } while (choice != 0);
    quit = true;
#endif
    updateThread.join();
    return EXIT_SUCCESS;
}

void Node::sendFrame(Frame &frame)
{
    can_frame canFrame;
    canFrame.can_dlc = frame.dlc;
    canFrame.can_id = frame.getCobID();
    memcpy(canFrame.data, frame.data, frame.dlc);
    int size = sizeof(canFrame);
    if (send(sock, &canFrame, size, 0) != size)
    {
        perror("Write");
        exit(EXIT_FAILURE);
    }
}

uint32_t Node::getTime_us()
{
    return (uint32_t)chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}

bool ObjectDictionnary::saveData(uint8_t /*parameterGroup*/)
{
    ofstream f(FILENAME, ios::out | ios::binary);
    if (!f)
        return false;
    f.write((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    return true;
}

bool ObjectDictionnary::loadData(uint8_t /*parameterGroup*/)
{
    ifstream f(FILENAME, ios::in | ios::binary);
    if (!f)
        return false;
    f.read((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    node.pdo().reloadTPDO();
    node.pdo().reloadRPDO();
    return true;
}

bool ObjectDictionnary::restoreData(uint8_t /*parameterGroup*/)
{
    objects.entries.data = ObjectDictionnaryData();
    node.pdo().reloadTPDO();
    node.pdo().reloadRPDO();
    return true;
}
