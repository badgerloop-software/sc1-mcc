#ifndef __GPIO_H__
#define __GPIO_H__

#include "can.h"
#include "mbed.h"
#include "stdint.h"
#include <cstdint>
#include "common.h"

/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and RPM at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(std::chrono::milliseconds pollPeriodMS);

/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
//  Returns 0 on success, -1 on failure
int disableGPIO(void);

extern uint16_t curGPIO;
extern DigitalOut Direction;


#endif