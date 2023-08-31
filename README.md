# CANopen
## Introduction
This is a C++14 written CANopen slave library. It is based on the official [CIA 301 CANopen application layer and communication profile](https://www.can-cia.org/groups/specifications/) document.  
This implementation features the following:
- Automatic generation of the object dictionnary as a header file from a device's EDS file.
- Non volatile storage of object dictionnary.
- NMT slave that can be controlled by a master or by the application.
- SDO server that supports regular and block transactions.
- PDO for both transmission and reception that supports dynamic PDO mapping and RTR.
- Heartbeat producer.
- Sync consumer that triggers TPDO transmission.
- Emergency producer with managed error register and pre-defined error field.

## Getting Started
Before compiling anything, it is mandatory to generate the object dictionnary header file.
A Python script is provided in the /generator folder.
Before using it, make sure to install the required packages from the requirements.txt file :
```bash
pip3 -r requirements.txt
```
You can run the generator.py script by providing the path of the EDS file and the node ID :
```bash
python3 generator.py example.eds 1
```
The node ID is required because it may be referenced in the EDS file, for example for PDO COB-ID.
The script will do some basic verification, such as missing mandatory objects, missing mapped objects, or unsupported data types.

**Keep in mind that the verification performed is basic and as such you are responsible for providing a well-build EDS file !**  

On success, the program will create a file named od.hpp containing the custom object dictionnary class.
It is a mandatory dependency for the rest of the library.

## Object Dictionnary
The object dictionnary is created from an associated EDS file. Because it is converted into a header file, it is statically allocated and its size is known at compile time.
The dictionnary is composed by an array of objects. Each object is identified by a hexadecimal index. Objects are themselves composed by an array of entries. Each entry is identified by a subindex.
The entries hold the data source, read/write access and data size. To access the data from the dictionnary, the 16 bits index of the object and the entry 8 bits subindex are used.

The CANopen standard defines a list of object types, such as VAR, RECORD or ARRAY. These notions are used only during the dictionnary rendering phase, but are otherwise meaningless.
Data is always accessed using the index/subindex multiplexer, so the object type doesn't matter at all. For VAR objects, data is simply located at subindex 0.

The standard also defines a list of object dictionnary data types.
For simplicity, this library supports all of the "primitive" data types that have a direct C type equivalent, with some tweaks.  
Here is the list of supported data types and their assigned C type :

Code    | Name              | C Type
------- | ----------------- | -----
0x01    | BOOLEAN           | bool
0x02    | INTEGER8          | int8_t
0x03    | INTEGER16         | int16_t
0x04    | INTEGER32         | int32_t
0x05    | UNSIGNED8         | uint8_t
0x06    | UNSIGNED16        | uint16_t
0x07    | UNSIGNED32        | uint32_t
0x08    | REAL32            | float
0x09    | VISIBLE_STRING    | uint8_t[ ]
0x0A    | OCTET_STRING      | uint8_t[ ]
0x0B    | UNICODE_STRING    | uint8_t[ ]
0x0C    | TIME_OF_DAY       | uint64_t
0x0D    | TIME_DIFFERENCE   | uint64_t
0x10    | INTEGER24         | int32_t
0x11    | REAL64            | double
0x12    | INTEGER40         | int64_t
0x13    | INTEGER48         | int64_t
0x14    | INTEGER56         | int64_t
0x15    | INTEGER64         | int64_t
0x16    | UNSIGNED24        | uint32_t
0x18    | UNSIGNED40        | uint64_t
0x19    | UNSIGNED48        | uint64_t
0x1A    | UNSIGNED56        | uint64_t
0x1B    | UNSIGNED64        | uint64_t

All of the non-regular integer types are automatically cast to the higher regular type, so for example INTEGER40 will be converted to INTEGER64, and so on.

For the sake of simplicity and versatility, there is no difference between the 3 string types. They are all treated as an utf-8 encoded char array, meaning that nearly every modern character can be stored.
This behaviour may not be compatible with all master implementations for non-ascii characters. Also, UNICODE_STRING is expected to have characters encoded on 16 bits each, so watch out for compatibility.

The DOMAIN type is not supported. This type is usually manufacturer specific, and is typically linked to external files that should not be stored directly in the dictionnary.

A list of pre-defined objects is also defined by the standard, some of which are mandatory for any CANopen application.
This library only takes standard objects into account. Device profile or manufacturer specific objects can be added manually.  
A lot of objects are not implemented as they are not related to slave functionnalities. More details under the Limitations section.  
Here is the list of standard objects, with unsupported ones being crossed :

Index   | Name                                           
------- | -----
0x1000  | Device type         
0x1001  | Error register         
0x1002  | Manufacturer status register
0x1003  | Pre-defined error field
~~0x1005~~  | ~~COB-ID SYNC~~
~~0x1006~~  | ~~Communication cycle period~~
0x1007  | Synchronous window length
0x1008  | Manufacturer device name
0x1009  | Manufacturer hardware version
0x100A  | Manufacturer software version
0x100C  | Guard time
~~0x100D~~  | ~~Life time factor~~
0x1010  | Store parameters
0x1011  | Restore default parameters
~~0x1012~~  | ~~COB-ID TIME~~
~~0x1013~~  | ~~High resolution time stamp~~
~~0x1014~~  | ~~COB-ID EMCY~~
~~0x1015~~  | ~~Inhibit Time EMCY~~
~~0x1016~~  | ~~Consumer heartbeat time~~
0x1017  | Producer heartbeat time
0x1018  | Identity Object
0x1019  | Synchronous counter overflow value
~~0x1020~~  | ~~Verify configuration~~
~~0x1021~~  | ~~Store EDS~~
0x1022  | Storage format
~~0x1023~~  | ~~OS command~~
~~0x1024~~  | ~~OS command mode~~
~~0x1025~~  | ~~OS debugger interface~~
~~0x1026~~  | ~~OS prompt~~
~~0x1027~~  | ~~Module list~~
~~0x1028~~  | ~~Emergency consumer~~
~~0x1200~~  | ~~1st SDO server parameter~~
...     | ...
~~0x127F~~  | ~~128th SDO server parameter~~
~~0x1280~~  | ~~1st SDO client parameter~~
...     | ...
~~0x12FF~~  | ~~128th SDO client parameter~~
0x1400  | 1st RPDO communication parameter
...     | ...
0x15FF  | 512th RPDO communication parameter
0x1600  | 1st RPDO mapping parameter
...     | ...
0x17FF  | 512th RPDO mapping parameter
0x1800  | 1st TPDO communication parameter
...     | ...
0x19FF  | 512th TPDO communication parameter
0x1A00  | 1st TPDO mapping parameter
...     | ...
0x1BFF  | 512th TPDO mapping parameter
~~0x1FA0~~  | ~~Object scanner list~~
...     | ...
~~0x1FCF~~  | ~~Object scanner list~~
~~0x1FD0~~  | ~~Object dispatching list~~
...     | ...
~~0x1FFF~~  | ~~Object dispatching list~~

## Hardware Interfacing
This library is meant to be device-agnostic so it may run on any target. As such, it is necessary for the user to implement an interface between the library and the device.
The interface is done by simply writing the function definition of a few class methods :
- `Node::sendFrame`: send a frame to the CAN network
- `Node::getTime_us`: called for internal timing
- `ObjectDictionnary::saveData`: save object dictionnary data to non-volatile memory
- `ObjectDictionnary::loadData`: load object dictionnary data from non-volatile memory
- `ObjectDictionnary::restoreData`: reset object dictionnary data to default

Examples can be found in the example.cpp file under the /example folder. These methods are documented in their header file as well.

```cpp
void Node::sendFrame(Frame &frame)
{
    // This example uses Linux SocketCan.
    can_frame canFrame;
    // Simply copy CANopen frame fields to regular CAN frame.
    canFrame.can_dlc = frame.dlc;
    canFrame.can_id = frame.getCobID();
    memcpy(canFrame.data, frame.data, frame.dlc);
    send(sock, &canFrame, sizeof(canFrame), 0)
}
```
```cpp
uint32_t Node::getTime_us()
{
    // This example uses std::chrono to get elapsed time in µs.
    // Returned time value doesn't have to be absolute, so a µs precise clock/counter value is also valid.
    // It HAS to wrap around 0xFFFFFFFF though.
    return (uint32_t)chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch()).count();
}
```
```cpp
bool ObjectDictionnary::saveData(uint8_t parameterGroup)
{
    // This example uses a file to store data.
    // It will save everything, independent of the parameterGroup argument.
    ofstream f(FILENAME, ios::out | ios::binary);
    if (!f) return false;
    // The dictionnary's data structure is located at objects.entries.data.
    // This is the structure that needs to persist.
    f.write((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    // Return true is saving was successful, false otherwise.
    return true;
}
```
```cpp
bool ObjectDictionnary::loadData(uint8_t parameterGroup)
{
    // This example uses a file to load data.
    // It will load everything, independent of the parameterGroup argument.
    ifstream f(FILENAME, ios::in | ios::binary);
    if (!f) return false;
    // The dictionnary's data structure is located at objects.entries.data.
    // This is the structure that needs to persist.
    f.read((char *)&this->objects.entries.data, sizeof(this->objects.entries.data));
    f.close();
    // Reload PDOs according to potentially updated mapping parameters.
    node.pdo().reloadTPDO();
    node.pdo().reloadRPDO();
    // Return true is loading was successful, false otherwise.
    return true;
}
```
```cpp
bool ObjectDictionnary::restoreData(uint8_t parameterGroup)
{
    // This example uses the ObjectDictionnaryData constructor to reset data.
    // It will reset everything, independent of the parameterGroup argument.
    objects.entries.data = ObjectDictionnaryData();
    // Reload PDOs according to potentially updated mapping parameters.
    node.pdo().reloadTPDO();
    node.pdo().reloadRPDO();
    // Return true is restoring was successful, false otherwise.
    return true;
}
```

## Usage Guide
Create a node
Access data from the object dictionnary

## Adding Objects



## Limitations
Despite the majority of the CANopen specification being well implemented, there are still some limitations.
- The TIME object is not supported.
- PDO mapping is limited to 8 objects, as granularity is set to 8.
- The generic pre-defined connection set is used, so custom COB-IDs for most objects are not supported (see p.80).
- Because of the generic connection set, mutation of COB-IDs during runtime is unsupported.
- Multiple RPDOS above default 4 are not supported, but multiple TPDOS (above default 4) are supported.
- MPDOs are not supported
- Multiple SDOs not supported.
- Node guarding is not supported.
- OS commands are not supported.
- If object 1029 (Error behaviour) is present, only subindex 1 (communication error) is accounted for, as other entries are device specific.
- The SYNC object has an internal counter that is incremented on each SYNC message. If the data contains a counter value, it is copied to the internal counter, otherwise the internal counter is incremented up to its maximum value.
The maximum value depends on whether or not object 1019 is present: if it is, the max value will be in range 2 to 240 (0 will set it to 240) based on the value of object 1019. If not, standard 240.
- LSS is not supported.
- Object flags (ObjFlags) is not supported.
- SDO block transfer CRC and PST not supported.



Ignored objects :
- Object 1005: standard SYNC COB-ID is used
- Object 1012: standard TIME COB-ID is used
- Object 1014: standard EMCY COB-ID is used
- Object 1006: sync producer feature
- Object 100D: node guarding unsupported
- Object 1013: not consumed and not published
- Object 1015: 
- Object 1016: heartbeat consumer feature
- Object 1020: 
- Object 1021: DOMAIN unsupported
- Objects 1023: OS commands unsupported
- Objects 1027: modular devices unsupported
- Object 1028: emergency consumer feature
- Objects 1200 to 127F: multiple SDO unsupported
- Objects 1280 to 12FF: multiple SDO unsupported
- Objects 1FA0 to 1FFF: MPDO unsupported
