/*****************************************************************//**
 * @file main.cpp
 *
 * @brief main application for inclination sensing
 *
 * @author Nick Storti
 * @version v1.0: initial release
 *********************************************************************/

// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "spi_core.h"
#include <sys/_stdint.h>
#include <math.h>
#include <cassert>

int main() {
    const uint8_t RD_CMD = 0x0b;
    const uint8_t WR_CMD = 0x0a;
    const uint8_t PART_ID_REG = 0x02;
    const uint8_t DATA_REG = 0x08;
    const uint8_t POWER_CTL_REG = 0x2d;
    const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

    int8_t xraw, yraw, zraw;
    float x, y, z;
    int id;

    GpoCore led_p(get_slot_addr(BRIDGE_BASE, S2_LED));
    SpiCore spi_p(get_slot_addr(BRIDGE_BASE, S9_SPI));

    spi_p.set_freq(400000);
    spi_p.set_mode(0, 0);

    // check part id
    spi_p.assert_ss(0);    // activate
    spi_p.transfer(RD_CMD);  // for read operation
    spi_p.transfer(PART_ID_REG);  // part id address
    id = (int) spi_p.transfer(0x00);
    spi_p.deassert_ss(0);
    assert(id == 0xf2);

    // activate measurement mode in power control register
    spi_p.assert_ss(0);
    spi_p.transfer(WR_CMD);
    spi_p.transfer(POWER_CTL_REG);  // power control register
    spi_p.transfer(0b00000010);
    spi_p.deassert_ss(0);

    while(1) {
        // read 8-bit x/y/z g values once
        spi_p.assert_ss(0);    // activate
        spi_p.transfer(RD_CMD);  // for read operation
        spi_p.transfer(DATA_REG);  
        xraw = spi_p.transfer(0x00);
        yraw = spi_p.transfer(0x00);
        zraw = spi_p.transfer(0x00);
        spi_p.deassert_ss(0);

        // process raw valules 
        x = (float) xraw / raw_max;
        y = (float) yraw / raw_max;
        z = (float) zraw / raw_max;

        // display orientation
        if(abs(x) >= abs(y)) {
            if(x >= 0) {
                uart.disp("orientation: 0");
                led_p.write(1);
            }
            else {
                uart.disp("orientation: 180");
                led_p.write(4);
            }
        }
        else {
            if(y >= 0) {
                uart.disp("orientation: 90");
                led_p.write(2);
            }
            else {
                uart.disp("orientation: 270");
                led_p.write(8);
            }
        }
        uart.disp("\n\r");
        sleep_ms(100);
    }

    return 0;
}