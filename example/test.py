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

index = 0x604A
subIndex = 1
value = 650
bytelen = 8

try: 
    print(int.from_bytes(node.sdo.upload(index, subIndex), 'little'))
    node.sdo.download(index, subIndex, value.to_bytes(bytelen, 'little'))
    print(int.from_bytes(node.sdo.upload(index, subIndex), 'little'))
    # network.send_message(0x604, bytes([0x40, 0x4A, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00]))
except (canopen.SdoCommunicationError, canopen.SdoAbortedError)  as e: print(e)



network.disconnect()
