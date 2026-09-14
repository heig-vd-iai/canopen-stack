#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <thread>

#include "FilePersistence.hpp"
#include "SocketCanTransport.hpp"
#include "full-node.hpp"
#include "hal/remote-objects.hpp"
#include "od/domainHandler.hpp"

using namespace CANopen;

namespace {

volatile std::sig_atomic_t quit = 0;

/** Keeps the DOMAIN object 0x2002 in a fixed buffer for block transfers. */
class BufferDomain : public DomainHandler {
   public:
    SDOAbortCodes beginDownload(int32_t, uint32_t size) override {
        if (size > sizeof(buffer))
            return SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh;
        return SDOAbortCode_OK;
    }
    SDOAbortCodes downloadChunk(int32_t, uint32_t offset, const uint8_t *bytes,
                                uint32_t length) override {
        if (offset + length > sizeof(buffer))
            return SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh;
        memcpy(buffer + offset, bytes, length);
        return SDOAbortCode_OK;
    }
    SDOAbortCodes endDownload(int32_t, uint32_t size) override {
        stored = size;
        printf("Domain 0x2002: %u bytes received\n",
               static_cast<unsigned>(size));
        return SDOAbortCode_OK;
    }
    void abortDownload(int32_t) override {}
    uint32_t uploadSize(int32_t) override { return stored; }
    SDOAbortCodes uploadChunk(int32_t, uint32_t offset, uint8_t *bytes,
                              uint32_t length) override {
        if (offset + length > stored)
            return SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh;
        memcpy(bytes, buffer + offset, length);
        return SDOAbortCode_OK;
    }

   private:
    uint8_t buffer[4096] = {0};
    uint32_t stored = 0;
};

SocketCanTransport transport;
FilePersistence persistence;
NullRemote remote;
BufferDomain domain;

FullNode canopen(transport, persistence, remote);

}  // namespace

int main(int argc, char *argv[]) {
    const char *interface = argc > 1 ? argv[1] : "vcan0";
    transport.setInterface(interface);
    signal(SIGINT, [](int) { quit = 1; });

    canopen.node.odAccessor().setDomainHandler(&domain);
    canopen.init();
    if (!transport.isOpen()) return 1;
    printf("Node %u on %s, Ctrl-C to stop\n",
           static_cast<unsigned>(canopen.node.nodeId), interface);

    while (!quit) {
        canopen.update();
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
    return 0;
}
