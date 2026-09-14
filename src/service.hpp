/**
 * Contains the declaration of the Service interface.
 */
#pragma once

#include <cstdint>

#include "enums.hpp"
#include "nmt/states.hpp"

#ifndef CANOPEN_MAX_SERVICES
#define CANOPEN_MAX_SERVICES 8
#endif

namespace CANopen {

class Frame;

/**
 * A CANopen protocol service attached to a Node.
 * The application builds the services it needs and attaches them; the node
 * dispatches frames, time and NMT state changes to them.
 */
class Service {
   public:
    virtual ~Service() = default;

    /**
     * Resolve the object dictionary entries used by this service.
     * Called once by Node::init(), after the hardware is initialized and
     * before the NMT state machine leaves Initialisation.
     */
    virtual void init() {}

    /**
     * Tell whether this service processes frames of the given function code.
     * @param functionCode Function code of an incoming frame.
     */
    virtual bool consumes(FunctionCodes functionCode) const = 0;

    /**
     * Process an incoming frame whose function code this service consumes.
     * @param frame Frame to be processed.
     * @param now_us Timestamp in microseconds of the frame reception.
     */
    virtual void onFrame(Frame &frame, uint32_t now_us) = 0;

    /**
     * Run the periodic work of this service.
     * @param now_us Current timestamp in microseconds.
     */
    virtual void update(uint32_t now_us) = 0;

    /**
     * React to an NMT state change.
     * @param state The state the node just entered.
     */
    virtual void onNmtState(NMTStates state) = 0;
};

/**
 * Publishes NMT state changes to the attached services.
 * Implemented by Node, used by the NMT state machine.
 */
class ServiceBus {
   public:
    virtual ~ServiceBus() = default;
    virtual void publishNmtState(NMTStates state) = 0;
};
}  // namespace CANopen
