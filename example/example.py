#!/usr/bin/python3
import canopen



class CustomNode(canopen.Node):
    def __init__(self, node_id: int, object_dictionary: str, load_od: bool = False):
        super().__init__(node_id, object_dictionary, load_od)
        for i in self.tpdo: self.tpdo[i].add_callback(self._on_TPDO)
        self.emcy.add_callback(self._on_EMCY)

    def _on_TPDO(self, message):
        print(f"{message.name}:")
        for var in message: print(f"{var.name} : {var.phys}")

    def _on_EMCY(self, emcy): print(f"Emergency code 0x{emcy.code:04X}: {emcy.get_desc()}")

    def print_errorRegister(self): print(f"[1001][Error register]: {self.sdo[0x1001].phys:#010b}")
    
    def print_errorField(self): print(f"[1003][Pre-defined error field]: {', '.join([f'0x{var.phys:X}' for var in self.sdo[0x1003].values()])}")

    def saveParameters(self): self.sdo[0x1010][1].raw = "save".encode()

    def restoreParameters(self): self.sdo[0x1011][1].raw = "load".encode()

    def setHeartbeatTime(self, value_ms: int): self.sdo[0x1017].phys = value_ms

 

node = CustomNode(1, '../generator/example.eds')
network = canopen.Network()
network.add_node(node)
network.connect(channel='vcan0', bustype='socketcan')
try:
    pass

    ## Block download
    data = ".susir lev cnun adauselam regetni oel seicirtlu eatiV .des sucal isin des essidnepsuS .sutcel teuqila di rotrot hbiN .iud ucra eranro sarc des taptulov orebil tidnalb deS .siruam sullet tipicsus eativ isin eugnoc cnuN .tema tis surup sittigas euqsiuq tsmutcid aetalp essatibah caH .niorp adauselam reprocmallu alugil te neipas ranivlup cnuN .teidrepmi silef euqsirelecs ue etatupluv sutem nI .te subicuaf neipas assam cenod merol rutetcesnoC .naenea sucnohr siruam eugnoc eativ susruc ucra a muspI .gnicsipida rutetcesnoc tema tis rolod muspi merol tse tegE .otsuj orebil man arreviv repmes tauqesnoC .ropmet assam ni surup subicuaf mauqilA .saneceam taptulov tidnalb mine tu oidO .euqen mine regetni mutnemele ranivlup subicuaF .saneceam satsege siprut ca semaf adauselam te suteN .euqsetnellep mauq supmet eativ euqen rotcua repmes sarc a sillavnoC .auqila angam erolod te erobal tu tnudidicni ropmet domsuie od des ,tile gnicsipida rutetcesnoc ,tema tis rolod muspi meroL".encode()
    with node.sdo[0x1F51].open("wb", size=len(data), block_transfer=True) as f:
        f.write(data)

    # Block upload
    with node.sdo[0x1F51].open("rb") as f:
        print(f.read(1024).decode())

    ## Configure TPDO
    # i = 1
    # node.tpdo.read()
    # node.tpdo[i].clear()
    # node.tpdo[i].trans_type = 0xFE
    # node.tpdo[i].event_timer = 1000
    # node.tpdo[i].inhibit_time = 10 * 1000
    # node.tpdo[i].enabled = True
    # node.tpdo[i].add_variable(0x6048, 1)
    # node.tpdo.save()

    ## NMT 
    # node.nmt.state = "PRE-OPERATIONAL"
    # node.nmt.state = "OPERATIONAL"
    # node.nmt.state = "STOPPED"
    # node.nmt.state = "RESET COMMUNICATION"
    # node.setHeartbeatTime(1000)

    ## Save or restore default parameters
    # node.restoreParameters()
    # node.saveParameters()

    ## Wait until KeyboardInterrupt
    # while True: pass

except KeyboardInterrupt: print()
except: pass
network.disconnect()
