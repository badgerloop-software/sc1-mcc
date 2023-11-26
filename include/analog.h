#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mutexless_analog.h"
#include "telemetry.h"

// defining the reference voltage for analog inputs
#define refrence_voltage 3.3

#define PEDAL_ON_THRESHOLD 0.25 // 1.5V/5V
#define PEDAL_RANGE 0.417  // 0.86 - 0.3, where 0.86 = 4.33V/5V is the max

// read all the analog inputs
void readAnalog();

// repeatedly call the function with the given parameter
void initAnalog(std::chrono::microseconds readSignalPeriod);

// set value of acc_out
void setAccOut(float acc);

#endif