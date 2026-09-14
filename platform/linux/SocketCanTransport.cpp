#include "SocketCanTransport.hpp"

#include <linux/can.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "frame.hpp"

using namespace CANopen;

SocketCanTransport::SocketCanTransport(const char *interface)
    : interface(interface) {}

SocketCanTransport::~SocketCanTransport() {
    if (sock >= 0) close(sock);
}

void SocketCanTransport::setInterface(const char *name) { interface = name; }

bool SocketCanTransport::isOpen() const { return sock >= 0; }

void SocketCanTransport::init() {
    if (sock >= 0) return;
    const unsigned ifindex = if_nametoindex(interface);
    if (ifindex == 0) {
        fprintf(stderr, "Unknown CAN interface \"%s\"\n", interface);
        return;
    }
    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("socket");
        return;
    }
    sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = static_cast<int>(ifindex);
    if (bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        sock = -1;
    }
}

void SocketCanTransport::sendFrame(const Frame &frame) {
    if (sock < 0) return;
    can_frame canFrame;
    memset(&canFrame, 0, sizeof(canFrame));
    canFrame.can_id = frame.getCobID();
    if (frame.rtr) canFrame.can_id |= CAN_RTR_FLAG;
    canFrame.can_dlc = frame.dlc;
    memcpy(canFrame.data, frame.data, frame.dlc);
    if (write(sock, &canFrame, sizeof(canFrame)) != sizeof(canFrame)) {
        perror("write");
    }
}

bool SocketCanTransport::receiveFrame(Frame &frame) {
    if (sock < 0) return false;
    can_frame canFrame;
    const ssize_t received =
        recv(sock, &canFrame, sizeof(canFrame), MSG_DONTWAIT);
    if (received != static_cast<ssize_t>(sizeof(canFrame))) {
        if (received < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("recv");
        }
        return false;
    }
    if (canFrame.can_id & (CAN_EFF_FLAG | CAN_ERR_FLAG)) return false;
    frame =
        Frame::fromCobId(static_cast<uint16_t>(canFrame.can_id & CAN_SFF_MASK));
    frame.rtr = (canFrame.can_id & CAN_RTR_FLAG) != 0;
    frame.dlc =
        canFrame.can_dlc > CAN_DATA_LENGTH ? CAN_DATA_LENGTH : canFrame.can_dlc;
    memcpy(frame.data, canFrame.data, frame.dlc);
    return true;
}

uint32_t SocketCanTransport::getTime_us() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    const uint64_t us = static_cast<uint64_t>(now.tv_sec) * 1000000u +
                        static_cast<uint64_t>(now.tv_nsec) / 1000u;
    return static_cast<uint32_t>(us);
}
