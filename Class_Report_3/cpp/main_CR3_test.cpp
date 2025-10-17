/*****************************************************************//**
 * @file main_CR3_test.cpp
 *
 * @brief Basic test of Blinking LED MMIO core
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 ********************************************************************/

#define _DEBUG
#include "chu_init.h"
#include "Blinking_LED.h"

int main() {

    BlinkingLEDCore dut(get_slot_addr(BRIDGE_BASE, S4_USER));
    sleep_ms(10000);

    while(1) {
        dut.SetRate_ms(1000, 0);
        dut.SetRate_ms(2000,1);
        dut.SetRate_ms(125, 2);
        dut.SetRate_ms(345,3);

        sleep_ms(15000);

        dut.SetRate_ms(100, 0);
        dut.SetRate_ms(3500,1);
        dut.SetRate_ms(1345, 2);
        dut.SetRate_ms(500,3);

        sleep_ms(15000);
    }
}
