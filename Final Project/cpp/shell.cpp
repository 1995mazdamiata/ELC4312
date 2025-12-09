/*****************************************************************//**
 * @file shell.cpp
 *
 * @brief implementation of Shell class
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 ********************************************************************/

#include "shell.h"

Shell::Shell() : led(get_slot_addr(BRIDGE_BASE, S2_LED)),
                  sw(get_slot_addr(BRIDGE_BASE, S3_SW)),
                  sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG)),
                  adxl362(get_slot_addr(BRIDGE_BASE, S9_SPI)),
                  adt7420(get_slot_addr(BRIDGE_BASE, S10_I2C)),
                  ps2(get_slot_addr(BRIDGE_BASE, S11_PS2)){

    //turn off sseg
    for (int i = 0; i < 8; i++) {
        sseg.write_1ptn(0xff, i);
    }
    sseg.set_dp(0x00);
}

Shell::~Shell(){
}

int Shell::get_command() {
    char commands[NUM_COMMANDS][MAX_STRING] = {"temp", "accl", "swch", "leds", "sseg"};
    char command[MAX_STRING];
    char ch;
    bool valid = false;
    int idx = 0;
    int out = -1;

    uart.disp("Enter a command:\r\n");

    while (out < 0) {
        if (ps2.get_kb_ch(&ch)) {
            if(ch == 0x0d) {
                for(size_t i = 0; i < NUM_COMMANDS; i++) {
                    if (strcmp(command, commands[i]) == 0) {
                        valid = true;
                    }
                }

                if(valid) {
                    if(strcmp(command, commands[READ_TEMP]) == 0) {
                        out = READ_TEMP;
                    }
                    else if(strcmp(command, commands[READ_ACCEL]) == 0) {
                        out = READ_ACCEL;
                    }
                    else if(strcmp(command, commands[READ_SW]) == 0) {
                        out = READ_SW;
                    }
                    else if(strcmp(command, commands[LED_ON]) == 0) {
                        out = LED_ON;
                    }
                    else {
                        out = PRINT_SSEG;
                    }
                }

                else {
                    uart.disp("\r\nInvalid command. Try again.");
                }

                idx = 0;
                command[idx] = '\0';
                uart.disp("\r\n");
            }

            else {
                uart.disp(ch);

                if(idx < MAX_STRING - 1) {
                    command[idx] = ch;
                    command[idx + 1] = '\0';
                    idx++;
                }
                else {
                    uart.disp("\r\nCommand too long. Try again.\r\n");
                    idx = 0;
                    command[idx] = '\0';
                }
            }
        }
    }
    
    return out;
}
 
void Shell::display_temp() {
    const uint8_t DEV_ADDR = 0x4b;
    uint8_t wbytes[2], bytes[2];
    uint16_t tmp;
    float tmpC;
    float tmpF;
    bool valid = false;
    char ch;

    // get unit from user
    uart.disp("C or F?\r\n");
    while(!valid) {
        if (ps2.get_kb_ch(&ch)) { 
            uart.disp(ch);
            uart.disp("\r\n");

            if (ch != 'C' && ch != 'c' &&
                ch != 'F' && ch != 'f') {
                uart.disp("Invalid unit. Try again.\r\n");
            }
            else { valid = true; }
        }
    }

    // read adt7420 id register to verify device existence
    wbytes[0] = 0x0b;
    adt7420.write_transaction(DEV_ADDR, wbytes, 1, 1);
    adt7420.read_transaction(DEV_ADDR, bytes, 1, 0);
    
    // read 2 bytes
    wbytes[0] = 0x00;
    adt7420.write_transaction(DEV_ADDR, wbytes, 1, 1);
    adt7420.read_transaction(DEV_ADDR, bytes, 2, 0);

    // conversion
    tmp = (uint16_t) bytes[0];
    tmp = (tmp << 8) + (uint16_t) bytes[1];
    if (tmp & 0x8000) {
        tmp = tmp >> 3;
        tmpC = (float) ((int) tmp - 8192) / 16;
    } else {
        tmp = tmp >> 3;
        tmpC = (float) tmp / 16;
    }
    
    tmpF = (tmpC * 1.8) + 32;

    if(ch == 'C' || ch == 'c') {
        uart.disp("Temperature (C): ");
        uart.disp(tmpC);
    }
    else {
        uart.disp("Temperature (F): ");
        uart.disp(tmpF);
    }
    uart.disp("\n\r");
}

void Shell::display_accl() {
    const uint8_t RD_CMD = 0x0b;
    const uint8_t WR_CMD = 0x0a;
    const uint8_t DATA_REG = 0x08;
    const uint8_t POWER_CTL_REG = 0x2d;
    const float raw_max = 127.0 / 2.0;  //128 max 8-bit reading for +/-2g

    int8_t xraw, yraw, zraw;
    float x, y, z;

    adxl362.set_freq(400000);
    adxl362.set_mode(0, 0);

    // activate measurement mode in power control register
    adxl362.assert_ss(0);
    adxl362.transfer(WR_CMD);
    adxl362.transfer(POWER_CTL_REG);  // power control register
    adxl362.transfer(0b00000010);
    adxl362.deassert_ss(0);

    // read 8-bit x/y/z g values once
    adxl362.assert_ss(0);    // activate
    adxl362.transfer(RD_CMD);  // for read operation
    adxl362.transfer(DATA_REG);  //
    xraw = adxl362.transfer(0x00);
    yraw = adxl362.transfer(0x00);
    zraw = adxl362.transfer(0x00);
    adxl362.deassert_ss(0);
    x = (float) xraw / raw_max;
    y = (float) yraw / raw_max;
    z = (float) zraw / raw_max;
    uart.disp("x/y/z axis g values: ");
    uart.disp(x, 3);
    uart.disp(" / ");
    uart.disp(y, 3);
    uart.disp(" / ");
    uart.disp(z, 3);
    uart.disp("\n\r");
}

void Shell::display_swch() {
    char command[MAX_STRING];
    char exit[MAX_STRING] = "exit";
    char ch;
    int s = 0, prev_s;
    int idx = 0;
    bool done = false;

    uart.disp("Enter 'exit' to stop.\r\n");

    while(!done) {
        prev_s = s;
        s = sw.read();

        if(s != prev_s) {
            led.write(s);
        }

        if (ps2.get_kb_ch(&ch)) {
            if(ch == 0x0d) {

                if(strcmp(command, exit) == 0) {
                    done = true;
                }
                else {
                    uart.disp("\r\nInvalid command. Try again.");
                }

                idx = 0;
                command[idx] = '\0';
                uart.disp("\r\n");
            }

            else {
                uart.disp(ch);

                if(idx < MAX_STRING - 1) {
                    command[idx] = ch;
                    command[idx + 1] = '\0';
                    idx++;
                }
                else {
                    uart.disp("\r\nCommand too long. Try again.\r\n");
                    idx = 0;
                    command[idx] = '\0';
                }
            }
        }
    }
 }

void Shell::leds_on() {
    const int LENGTH = 16;
    char num[LENGTH];
    char ch;
    int n = 0;
    int num_idx = 0;
    bool done = false;
    bool valid = true;

    uart.disp("Enter 16 bit squence for LEDs.\r\n");     

    while(!done) {
        while(num_idx < LENGTH) {
            if (ps2.get_kb_ch(&ch)) {
                uart.disp(ch);
                num[num_idx] = ch;
                num_idx++;

                if(ch != '0' && ch != '1') {
                    valid = false;
                }
            }
        }
        uart.disp("\r\n");

        if(valid) {
            for(int i = 0; i < LENGTH; i++) {
                if(num[i] == '1') {
                    n += pow(2, LENGTH - 1 - i);
                }
            }
            led.write(n);
        }
        else {
            uart.disp("Invalid sequence.\r\n");
        }
        num_idx = 0;
        n = 0;
        valid = true;

        uart.disp("Again? (y/n)\r\n");
        while(!ps2.get_kb_ch(&ch)) {}

        uart.disp(ch);
        uart.disp("\r\n");        
        if(ch == 'n' || ch == 'N') {
            done = true;
            led.write(0);
        }
    }
}

void Shell::print_sseg() {
    const int LENGTH = 8;
    char num[LENGTH];
    char ch;
    int num_idx = 0;
    bool done = false;
    bool valid = true;

    uart.disp("Enter 8 decimal characters.\r\n");     

    while(!done) {
        while(num_idx < LENGTH) {
            if (ps2.get_kb_ch(&ch)) {
                uart.disp(ch);
                num[num_idx] = ch;
                num_idx++;

                if(ch != '0' && ch != '1' && ch != '2' && ch != '3' &&
                   ch != '4' && ch != '5' && ch != '6' && ch != '7' &&
                   ch != '8' && ch != '9') {
                    valid = false;
                }
            }
        }
        uart.disp("\r\n");

        if(valid) {
            for(int i = 0; i < LENGTH; i++) {
                sseg.write_1ptn(sseg.h2s(num[i] - '0'), LENGTH - 1 - i);
            }
        }
        else {
            uart.disp("Invalid sequence.\r\n");
        }
        num_idx = 0;
        valid = true;

        uart.disp("Again? (y/n)\r\n");
        while(!ps2.get_kb_ch(&ch)) {}
        
        uart.disp(ch);
        uart.disp("\r\n");
        if(ch == 'n' || ch == 'N') {
            done = true;
            for (int i = 0; i < LENGTH; i++) {
                sseg.write_1ptn(0xff, i);
            }
        }
    }
}

void Shell::run_app() {
    int command;

    command = this->get_command();

    if(command == READ_TEMP) {
        this->display_temp();
    }
    else if(command == READ_ACCEL) {
        this->display_accl();
    }
    else if(command == READ_SW) {
        this->display_swch();
    }
    else if(command == LED_ON) {
        this->leds_on();
    }
    else if(command == PRINT_SSEG) {
        this->print_sseg();
    }
    else {
        uart.disp("Invalid command. Try again.\r\n");
    }
}