#ifndef __GPIO_H__
#define __GPIO_H__

#include "mbed.h"
#include "stdint.h"


/// GPIO CAN Outputs
//  rps - Address: XXX
//  GPIO- Address: XXX
//    Bit 0  | Power
//    Bit 1  | Direction
//    Bit 2  | Brake
//    Bit 3  | Operation Mode
//    Bit 4  | Cruise On/Off
//    Bit 5  | Cruise Mode
//    Bit 6  | MC Status LED
//    Bit 7  | UNUSED
//    Bit 8  | UNUSED


/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and rpm at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(uint16_t pollPeriodMS, uint16_t rpsCalcPeriodMS);

/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
//  Returns 0 on success, -1 on failure
int disableGPIO(void);

#endif