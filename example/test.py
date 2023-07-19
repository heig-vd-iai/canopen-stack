from time import sleep
import canopen
import struct



network = canopen.Network()
node = canopen.Node(4, "../generator/example.eds")
# entries = [entry for entry in node.object_dictionary.items() if 0x1000 <= entry[0] <= 0x1FFF]
# for index, entry in entries: print(f"[%X] {entry.name}" % index)
network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    node.tpdo.read()



    # network.scanner.search()
    # sleep(0.05)
    # for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

    # for state in ['OPERATIONAL', 'STOPPED', 'INITIALISING']:
    #     node.nmt.state = state
    #     sleep(3)
except Exception as e: print(e)
network.disconnect()
