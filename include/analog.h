#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"

extern Semaphore queueSem;
extern CANMessage outputQueue[4];
extern EventFlags queueFlags;

/// Initializes analog update functions at specified period
//  Automatically queues updated value to can output
//  Returns 0 on success, -1 on failure
int initAnalog(std::chrono::milliseconds pollRateMS);

/// Disables all analog output functions
//  Run and reinit to change poll rate
//  Returns 0 on success, -1 on failure
int disableAnalog(void);

#endif