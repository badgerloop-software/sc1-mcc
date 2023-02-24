#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "can.h"
#include "gpio.h" // For state machine 

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR HIGHER LEVEL DETAILS
///////////////////////////////////////////////////////////////////////
// Current Analog Pins
// ---------------------------------
// Name        | Direction   | Pin    | CAN Offset  | MCC   | Source
// Accel In    | Input       | A0     | 3           | Input | Pin
// Brake In    | Input       | A1     | 4           | Input | Pin?
// Accel Out   | Output      | A4     | n/a         | Output| Accel In/CrzCntrl
///////////////////////////////////////////////////////////////////////

/// Initializes analog update functions at specified period
//  Updated values will be automatically added to can output queue
//  Returns 0 on success, -1 on failure
int initAnalog(std::chrono::milliseconds pollRateMS);


/// Disables all analog output functions
//  Run and reinit to change poll rate
//  Returns 0 on success, -1 on failure
int disableAnalog(void);

#endif