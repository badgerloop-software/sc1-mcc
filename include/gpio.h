#ifndef __GPIO_H__
#define __GPIO_H__

#include "mbed.h"
#include "stdint.h"
#include "can.h"

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR HIGHER LEVEL DETAILS
///////////////////////////////////////////////////////////////////////
// Current GPIO Pins
// ---------------------------------
// Name        | Direction   | CAN Offset  | Pin   
// SpdPulse/RPM| Input       | 0           | D1             
// GPIO Bitmap | Input       | 1           | D3 - D9, D11-D12  
///////////////////////////////////////////////////////////////////////
// GPIO Bitmap
// ---------------------------------
// Bit         | Name        | Pin(s)
// 0           | Power       | D7
// 1           | Direction   | D8
// 2           | Brake       | D12
// 3           | Eco         | D9
// 4           | Crz Enable  | D5/D6
// 5           | Crz Mode    | D3/D4
// 6           | MC Status   | D11
// 7-15        | Unused      |  
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