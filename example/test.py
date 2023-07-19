from time import sleep
import canopen
import struct


def callback(message):
    for var in message:
        print('%s = %d' % (var.name, var.raw))



network = canopen.Network()
node = canopen.Node(4, "../generator/example.eds")
# entries = [entry for entry in node.object_dictionary.items() if 0x1000 <= entry[0] <= 0x1FFF]
# for index, entry in entries: print(f"[%X] {entry.name}" % index)

# a = node.object_dictionary[0x1800].subindices[1].default
# print(a)
# exit()

network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    node.tpdo.read()
    # node.sdo.download(0x1800, 5, (0).to_bytes(2, 'little'))
    node.tpdo[2].add_callback(callback)
    node.tpdo[2].event_timer = 1000
    node.tpdo.save()
    network.nmt.state = "OPERATIONAL"
    sleep(3)




    # network.scanner.search()
    # sleep(0.05)
    # for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

    # for state in ['OPERATIONAL', 'STOPPED', 'INITIALISING']:
    #     node.nmt.state = state
    #     sleep(3)
except Exception as e: print(e)
network.disconnect()
