#pragma once

namespace CANopen {

/** Data type codes of the object dictionary. CiA301:2011§7.4.7 */
enum DataType {
    BOOLEAN = 0x01,
    INTEGER8 = 0x02,
    INTEGER16 = 0x03,
    INTEGER32 = 0x04,
    UNSIGNED8 = 0x05,
    UNSIGNED16 = 0x06,
    UNSIGNED32 = 0x07,
    REAL32 = 0x08,
    VISIBLE_STRING = 0x09,
    DOMAIN = 0x0F,
    REAL64 = 0x11,
    INTEGER64 = 0x15,
    UNSIGNED64 = 0x1B
};
}  // namespace CANopen
