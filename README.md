# CANopen
## Introduction
This is C++ written CANopen slave library. It was written based on the official [CIA 301 CANopen application layer and communication profile](https://www.can-cia.org/groups/specifications/) document.  
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
The entries hold the data source, read/write access and data size. To access the data from the dictionnary, the index of the object and the entry subindex are used.
*table of supported objects*

## Running The Example

## How To
### Create a node
### Access data from the object dictionnary


## Hardware Interfacing


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

Unsupported or aliased data types
- DOMAIN is not supported.
- VISIBLE_STRING is UTF-8 encoded, OCTET_STRING and UNICODE_STRING are aliases to VISIBLE_STRING.
- TIME_OF_DAY and TIME_DIFFERENCE are alisases to UNSIGNED64.

Tweaked or ignored objects
- Custom or mutable cob-ids are not supported. Some objects are affected:
    - 1005 is ignored (COB-ID SYNC).
    - 1012 is ignored (COB-ID time stamp).
    - 1014 is ignored (COB-ID EMCY).
- Object 1006 is ignored (sync producer feature).
- Object 100D is ignored (node guarding unsupported).
- Object 1013 is ignored (not consumed and not published).
- Object 1015 is ignored.
- Object 1016 is ignored (heartbeat consumer feature).
- Object 1020 is ignored.
- Object 1021 is ignored (DOMAIN unsupported).
- Objects 1023 to 1026 are ignored (OS commands unsupported).
- Objects 1027 is ignored (modular devices unsupported).
- Object 1028 is ignored (emergency consumer feature).
- Objects 1200 to 127F are ignored (multiple SDO unsupported).
- Objects 1280 to 12FF are ignored (multiple SDO unsupported).
