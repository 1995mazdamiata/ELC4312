/*****************************************************************//**
 * @file main_vanilla_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

#define _DEBUG
#include "chu_init.h"

int main() {

    while(1) {
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 0, 1000);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 1, 2000);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 2, 125);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 3, 345);

        sleep_ms(15000);

        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 0, 100);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 1, 3500);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 2, 1345);
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), 3, 500);

        sleep_ms(15000);
    }
}
