from time import sleep
import canopen
import struct


def callback(message):
    print("PDO received:")
    for var in message: 
        [a] = struct.unpack("<d", var.data)
        print('%s = %d' % (var.name, float(var.phys)))
        print(a)
    print()



network = canopen.Network()
node = canopen.Node(4, "../generator/example.eds")

index = 0x1800
subindex = 5

network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    # node.sdo.download(0x6041, 0, (0x69).to_bytes(2, 'little'))
    # node.sdo.download(0x6061, 0, (0x42).to_bytes(1, 'little'))
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)
    # node.sdo.download(index, subindex, (val + 1).to_bytes(2, 'little'))
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)

    network.nmt.state = "PRE-OPERATIONAL"
    node.tpdo[1].read()
    node.tpdo[1].clear()
    node.tpdo[1].add_variable(0x6048, 1)
    node.tpdo[1].transmission_type = 0xFE
    node.tpdo[1].event_timer = 1000
    node.tpdo[1].enabled = True
    node.tpdo[1].add_callback(callback)
    node.tpdo[1].save()
    sleep(0.01)
    network.nmt.state = "OPERATIONAL"

    # network.scanner.search()
    # sleep(0.05)
    # for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

    # for state in ['OPERATIONAL', 'STOPPED', 'INITIALISING']:
    #     node.nmt.state = state
    #     sleep(3)

    while True: pass
except KeyboardInterrupt: print()
except Exception as e: print(e)
network.disconnect()
