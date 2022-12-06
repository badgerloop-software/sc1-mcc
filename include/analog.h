#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "can.h"

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR IMPLEMENTATION AND MODIFICATION DETAILS
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