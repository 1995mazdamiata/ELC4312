/*****************************************************************//**
 * @file main.cpp
 *
 * @brief main loop for basic command line application
 *
 * @author Nicholas Storti
 * @version v1.0: initial release
 *********************************************************************/

#include "shell.h"

int main() {
    Shell shell;

    while (1) {
        shell.run_app();
    }
} //main