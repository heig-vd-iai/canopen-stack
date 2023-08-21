import canopen



class CustomNode(canopen.Node):
    def __init__(self, node_id: int, object_dictionary: str, load_od: bool = False):
        super().__init__(node_id, object_dictionary, load_od)
        for i in self.tpdo: self.tpdo[i].add_callback(self._on_TPDO)

    def _on_TPDO(self, message):
        print(message.name)
        for var in message: print(f"{var.name} : {var.phys}")

    def print_errorRegister(self): print(f"[1001][Error register] {self.sdo[0x1001].phys:#010b}")
    
    def print_errorField(self):
        print(f"[1003][Pre-defined error field]")
        for error in self.sdo[0x1003]:
            print(error)



node = CustomNode(1, '../generator/example.eds')
network = canopen.Network()
network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:


    node.print_errorRegister()
    node.print_errorField()




    ## Configure TPDO
    # i = 1
    # node.tpdo.read()
    # node.tpdo[i].clear()
    # node.tpdo[i].trans_type = 255
    # node.tpdo[i].event_timer = 500
    # node.tpdo[i].inhibit_time = 100
    # node.tpdo[i].enabled = True
    # node.tpdo[i].add_variable(0x6048, 1)
    # node.tpdo.save()

    ## NMT 
    # node.nmt.state = "PRE-OPERATIONAL"
    # node.nmt.state = "OPERATIONAL"
    # node.nmt.state = "STOPPED"
    # node.nmt.state = "RESET COMMUNICATION"

    ## Wait until KeyboardInterrupt
    # while True: pass

except KeyboardInterrupt: print()
except: pass
network.disconnect()
