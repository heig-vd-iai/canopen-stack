from time import sleep
import canopen
import struct


def callback(message):
    for var in message: print('%s = %d' % (var.name, var.raw))



network = canopen.Network()
node = canopen.Node(4, "../generator/example.eds")
# entries = [entry for entry in node.object_dictionary.items() if 0x1000 <= entry[0] <= 0x1FFF]
# for index, entry in entries: print(f"[%X] {entry.name}" % index)

# a = node.object_dictionary[0x1800].subindices[1].default
# print(a)
# exit()
index = 0x1800
subindex = 5

network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)
    # node.sdo.download(index, subindex, (val + 1).to_bytes(2, 'little'))
    # val = int.from_bytes(node.sdo.upload(index, subindex), 'little')
    # print(val)

    # node.sdo.download(0x1800, 2, (0xF1).to_bytes(1, 'little'))
    # node.sdo.download(0x1800, 2, (0xFB).to_bytes(1, 'little'))
    print("Before")
    print("COB-ID: " + hex(int.from_bytes(node.sdo.upload(0x1800, 1), 'little')))
    print("Inhibit time: " + str(int.from_bytes(node.sdo.upload(0x1800, 3), 'little')))
    print("After")
    node.tpdo.read()
    node.tpdo[1].add_callback(callback)
    node.tpdo[1].event_timer = 1000
    node.tpdo[1].enabled = False
    node.tpdo.save()
    print("COB-ID: " + hex(int.from_bytes(node.sdo.upload(0x1800, 1), 'little')))
    node.tpdo[1].inhibit_time = 200
    node.tpdo.save()
    print("Inhibit time: " + str(int.from_bytes(node.sdo.upload(0x1800, 3), 'little')))

    # network.nmt.state = "OPERATIONAL"
    # sleep(3)

    # network.scanner.search()
    # sleep(0.05)
    # for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

    # for state in ['OPERATIONAL', 'STOPPED', 'INITIALISING']:
    #     node.nmt.state = state
    #     sleep(3)
except Exception as e: print(e)
network.disconnect()
