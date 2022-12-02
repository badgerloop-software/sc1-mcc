#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"



/// Initializes analog update functions at specified period
//  Automatically queues updated value to can output
//  Returns 0 on success, -1 on failure
int analogInit(uint16_t pollRateMS);

/// Disables all analog output functions
//  Run and reinit to change poll rate
//  Returns 0 on success, -1 on failure
int disableAnalog(void);

#endif