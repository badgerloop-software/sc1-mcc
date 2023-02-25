#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "can.h"

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

extern uint8_t curState; // Track state machine to dictate ACC_OUT control

/// Initializes analog update functions at specified period
//  Updated values will be automatically added to can output queue
void initAnalog(std::chrono::milliseconds pollRateMS);


/// Disables all analog output functions
//  Run and reinit to change poll rate
void disableAnalog(void);

#endif