/*****************************************************************//**
 * @file Blinking_LED.h
 *
 * @brief Access blinking LED core and set blinking intervals for 
 *        the blinking LED's
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 ********************************************************************/

#ifndef _BLINKING_LED_H_INCLUDED
#define _BLINKING_LED_H_INCLUDED

#include "chu_io_rw.h"
#include "chu_io_map.h"

/**
 * Blinking LED Core driver
 * - set blinking interval for individual LED's
 *
 */
class BlinkingLEDCore {
    /**
    * register map
    *
    */
    enum {
        LED0_REG = 0,
        LED1_REG = 1,
        LED2_REG = 2,
        LED3_REG = 3
    };

public:
    /* methods */
    /**
    * constructor with default blinking interval values
    *
    */
    BlinkingLEDCore(uint32_t core_base_addr, uint32_t r0 = 1000, 
     uint32_t r1 = 1000, uint32_t r2 = 1000, uint32_t r3 = 1000);
    ~BlinkingLEDCore();

    /**
    *set blinking period in ms
    *
    */
    void SetRate_ms(uint32_t rate_ms, uint32_t led_idx);

private:
   uint32_t base_addr;
   uint32_t led_rate[4];
};


#endif  // _BLINKING_LED_H_INCLUDED