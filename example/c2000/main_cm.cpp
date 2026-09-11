#include "C2000Hardware.hpp"
#include "CANopen.hpp"
#include "sharedtypes.h"
extern "C" {
#include "cm.h"
}
#include <cstring>

void main(void) {
    CM_init();

    C2000Hardware hardware;

    CANopen::node.init(&hardware);

    while (true) {
        // DEVICE_DELAY_US(2000000);
        // GPIO_writePin(30, 0);
        // DEVICE_DELAY_US(2000000);
        // GPIO_writePin(30, 1);
        CANopen::node.update();
    }
}
