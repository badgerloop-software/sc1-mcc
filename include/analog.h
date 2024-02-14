#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mutexless_analog.h"
#include "telemetry.h"

// read all the analog inputs
void readAnalog();

// repeatedly call the function with the given parameter
void initAnalog(std::chrono::microseconds readSignalPeriod);

// set value of acc_out
void setAccOut(float acc);

#endif