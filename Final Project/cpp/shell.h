/*****************************************************************//**
 * @file shell.h
 *
 * @brief basic shell commands
 *
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 *********************************************************************/

#ifndef _SHELL_H_INCLUDED
#define _SHELL_H_INCLUDED

#include "chu_init.h"
#include "sseg_core.h"
#include "gpio_cores.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include <string.h>
#include <cmath>


const int NUM_COMMANDS = 5;
const int MAX_STRING = 5;

/**
 * class for basic shell commands
 *  - display temperature data
 *  - display accelerometer data
 *  - display switch data
 *  - illuminate led's
 *  - print to 7-segment display
 *
 */

class Shell {
public:
    /**
     * Commands
     *
     */
     enum {
         READ_TEMP = 0,
         READ_ACCEL = 1,
         READ_SW = 2,
         LED_ON = 3,
         PRINT_SSEG = 4
     };

    /* methods */

    /**
     * constructor
     *
     */
    Shell();
    ~Shell();

    /**
     * read text input and check for command
     *
     * @return integer corresponding to enumerated command
     *
     */
    int get_command();

    /**
     * display temperature data
     *
     */
    void display_temp();

    /**
     * display accelerometer data
     *
     */
    void display_accl();

    /**
     * display which switches are on
     *
     */
    void display_swch();

    /**
     * turn leds on in user specified sequence
     *
     */
    void leds_on();

    /**
     * print decimal values to 7-seg display
     *
     */
    void print_sseg();

    /**
     * handles function call sequence to run commands
     *
     */
    void run_app();

private:
    GpoCore led;
    GpiCore sw;
    SsegCore sseg;
    SpiCore adxl362;
    I2cCore adt7420;
    Ps2Core ps2;
};

#endif  // _SHELL_H_INCLUDED