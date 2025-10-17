/*****************************************************************//**
 * @file Blinking_LED.cpp
 *
 * @brief Implementation of BlinkingLEDCore class
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 ********************************************************************/

#include "Blinking_LED.h"

BlinkingLEDCore::BlinkingLEDCore(uint32_t core_base_addr, uint32_t r0, 
                                uint32_t r1, uint32_t r2, uint32_t r3) {
    this->base_addr = core_base_addr;

    led_rate[0] = r0;
    led_rate[1] = r1;
    led_rate[2] = r2;
    led_rate[3] = r3;

    for(int i = 0; i < 3; i++) {
        SetRate_ms(led_rate[i], i);
    }
}

BlinkingLEDCore::~BlinkingLEDCore() {
}

void BlinkingLEDCore::SetRate_ms(uint32_t rate_ms, uint32_t led_idx) {
    uint32_t reg_offset;

    switch (led_idx) {
        case LED0_REG: reg_offset = LED0_REG; break;
        case LED1_REG: reg_offset = LED1_REG; break;
        case LED2_REG: reg_offset = LED2_REG; break;
        case LED3_REG: reg_offset = LED3_REG; break;
        default: reg_offset = 5;
    }

    if((reg_offset < 5) && (rate_ms < 65536)) {
        led_rate[reg_offset] = rate_ms;
        io_write(get_slot_addr(BRIDGE_BASE, S4_USER), reg_offset, rate_ms);
    }
}