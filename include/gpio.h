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
// 0           | Power       | D7       | Output    | CAN?
// 1           | Direction   | D8       | Output    | CAN?
// 2           | DirectionOut| D7       | Output    | CAN?
// 3           | Brk_Status  | D12      | N/A       | Pin
// 4           | Eco         | D9       | Output    | CAN?
// 5           | Crz Enable  |          | N/A       | Pin
// 6           | Crz Mode A/B| D3/D4    | N/A       | Pin
// 7           | MC Status   | D11      | Input     | Pin?
// 15          | Unused      |          | N/A       | N/A
// NA          | Speed Pulse | D1       | Input     | Pin
///////////////////////////////////////////////////////////////////////


/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and RPM at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
void initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS);


/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
void disableGPIO(void);

#endif