#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "telemetry.h"

// defining the reference voltage for analog inputs
#define refrence_voltage 3.3

// read all the analog inputs
void readAnalog();

// repeatedly call the function with the given parameter
void initAnalog(std::chrono::microseconds readSignalPeriod);

#endif