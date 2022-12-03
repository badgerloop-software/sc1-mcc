#ifndef __GPIO_H__
#define __GPIO_H__

#include "mbed.h"
#include "stdint.h"


/// GPIO CAN Outputs
//  RPM - Address: 200
//  GPIO- Address: 201
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
//  Initializes functions which update GPIO and RPM at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(std::chrono::microseconds pollPeriodMS, std::chrono::microseconds rpmCalcPeriodMS);

/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
//  Returns 0 on success, -1 on failure
int disableGPIO(void);

#endif