#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "can.h"

/*
* Pedal voltage limits - Used to define percentage pressed
* Current values obtained via measuring output values from pressing & depressing pedal
* Update whenever the pedal -> MCC circuitry changes. 
*
* NOTE: Pedal outputs 5V. This data was observed using a 1Kohm / 2Kohm voltage divider.
*
* There's a small safety factor added - Ensures that a depressed pedal always returns 0% pressed
*/
#define PEDAL_NO_PRESS 0.529
#define PEDAL_FULL_PRESS 1.492

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR HIGHER LEVEL DETAILS
///////////////////////////////////////////////////////////////////////
// Current Analog Pins
// ---------------------------------
// Name        | Direction   | Pin    | CAN Offset
// Accel In    | Input       | A0     | 3
// Brake In    | Input       | A1     | 4
// Pedal In    | Input       | A5     | 5    NOTE: Pedal outputs 5V
// Accel Out   | Output      | A4     | n/a
///////////////////////////////////////////////////////////////////////

/// Initializes analog update functions at specified period
//  Updated values will be automatically added to can output queue
//  Returns 0 on success, -1 on failure
int initAnalog(std::chrono::milliseconds pollRateMS);

// Converts input voltage to a percent of pedal pressed, using measured
// voltage limits defined above
float calculate_pedal_press(float voltage);


/// Disables all analog output functions
//  Run and reinit to change poll rate
//  Returns 0 on success, -1 on failure
int disableAnalog(void);

#endif