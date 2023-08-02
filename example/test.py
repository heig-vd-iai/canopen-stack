from time import sleep, time
import canopen
import struct
import random


def callback(message):
    for var in message: print('%s = %f' % (var.name, var.phys))
    
# lastCalled = 0
# def callback2(message):
#     global lastCalled
#     t = time()
#     dt = t - lastCalled
#     lastCalled = t
#     print(round(dt / DT))


network = canopen.Network()
node = canopen.RemoteNode(4, "../generator/example.eds")

index = 0x1A00
subindex = 3

network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    # node.sdo.download(0x6041, 0, (0x69).to_bytes(2, 'little'))
    # node.sdo.download(0x6061, 0, (0x42).to_bytes(1, 'little'))
    # node.sdo.download(index, subindex, (4114).to_bytes(4, 'little'))
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)

    # network.nmt.state = "PRE-OPERATIONAL"
    # node.tpdo[1].read()
    # node.tpdo[1].clear()
    # node.tpdo[1].add_variable(0x6048, 1)
    # # node.tpdo[1].add_variable(0x6048, 2)
    # node.tpdo[1].trans_type  = 0xFE
    # node.tpdo[1].event_timer = 1000
    # node.tpdo[1].inhibit_time = 5000
    # node.tpdo[1].enabled = True
    # node.tpdo[1].add_callback(callback)
    # node.tpdo[1].save()
    # sleep(0.01)
    # network.nmt.state = "OPERATIONAL"
    # network.nmt.state = "OPERATIONAL"
    # network.nmt.state = "PRE-OPERATIONAL"
    network.nmt.state = "STOPPED"

    # node.tpdo[1].remote_request()
    # node.tpdo[1].start(10)

    # network.scanner.search()
    # sleep(0.05)
    # for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

    # for state in ['OPERATIONAL', 'STOPPED', 'INITIALISING']:
    #     node.nmt.state = state
    #     sleep(3)

    # network.sync.transmit()
    # network.sync.start(0.333)

    # counter = 0
    # t0 = 0
    # DT = 333e-3
    # while True: pass
    #     t0 = time()
    #     node.tpdo[1].remote_request()
    # #     counter = 1 if counter >= 255 else counter + 1
    # #     network.sync.transmit(counter)
    #     dt = random.random() + 1
    #     dt = 0.25
    #     while time() - t0 < dt: pass
    #     t1 = time()

except KeyboardInterrupt: print()
except Exception as e: print(e)
network.disconnect()
