import canopen
from time import sleep




states = ['OPERATIONAL', 'STOPPED', 'INITIALISING']


network = canopen.Network()
node = canopen.RemoteNode(4, "../generator/example.eds")
network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')


# This will attempt to read an SDO from nodes 1 - 127
# network.scanner.search()
# sleep(0.05)
# for node_id in network.scanner.nodes: print("Found node %d!" % node_id)

# for state in states:
#     node.nmt.state = state
#     sleep(0.1)

try: print(int.from_bytes(node.sdo.upload(0x604A, 1), 'little'))
except (canopen.SdoCommunicationError, canopen.SdoAbortedError)  as e: print(e)



network.disconnect()
