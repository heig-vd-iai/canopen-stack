/**
 * Composition of a node with every CiA301 service.
 * Applications that need a smaller stack build the services they want and
 * attach them to a Node themselves.
 */
#pragma once

#include "emergency.hpp"
#include "heartbeat.hpp"
#include "node.hpp"
#include "od_common.hpp"
#include "pdo.hpp"
#include "sdo/sdoServer.hpp"
#include "sync.hpp"

namespace CANopen {

/**
 * A node running the heartbeat, SDO, PDO, SYNC and EMCY services.
 * The application owns this object and keeps it alive for the whole life of
 * the program.
 */
class FullNode {
   public:
    FullNode(CanTransport &transport, Persistence &persistence,
             RemoteObjects &remote)
        : node(transport, persistence, remote),
          hb(node.od(), transport, node.nodeId),
          sdo(node.odAccessor(), transport, node.nodeId),
          pdo(node.od(), transport, remote, node.nodeId),
          sync(node.od(), &pdo),
          emcy(node.od(), transport, node.nmt(), node.nodeId) {
        node.attach(hb);
        node.attach(sdo);
        node.attach(pdo);
        node.attach(sync);
        node.attach(emcy);
        bindHeartbeat(hb);
        bindPdo(pdo);
        bindSync(sync);
        bindEmergency(emcy);
    }

    void init() { node.init(); }
    void update() { node.update(); }

    Node node;
    HB hb;
    SDO sdo;
    PDO pdo;
    SYNC sync;
    EMCY emcy;
};
}  // namespace CANopen
