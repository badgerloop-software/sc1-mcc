#ifndef __GPIO_H__
#define __GPIO_H__

#include "mbed.h"
#include "stdint.h"
#include "can.h"

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR HIGHER LEVEL DETAILS
///////////////////////////////////////////////////////////////////////
// GPIO Bitmap and Pins
// -----------------------------------------
// Bit         | Name        | Pin(s)   | MCC       | Source
// 0           | Power       | D0       | Output    | CAN?
// 1           | Direction   | D13      | Output    | CAN?
// 2           | Brk_Status  | D12      | N/A       | Pin
// 3           | Eco         | D9       | Output    | CAN?
// 4           | Crz Enable  | D5/D6    | N/A       | Pin
// 5           | Crz Mode    | D3/D4    | N/A       | Pin
// 6           | MC Status   | D11      | Input     | Pin?
// 7-15        | Unused      |          | N/A       | N/A
// NA          | Speed Pulse | D1       | Input     | Pin
///////////////////////////////////////////////////////////////////////


/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and RPM at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS);


/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
//  Returns 0 on success, -1 on failure
int disableGPIO(void);

#endif