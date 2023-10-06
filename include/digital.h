#ifndef __DIGITAL_H__
#define __DIGITAL_H__

#include "telemetry.h"

// read all the digital inputs
void readDigital();

// repeatedly call the function with the given parameter
void initDigital(std::chrono::microseconds readSignalPeriod);

// Sets value of direction (forward/reverse) output to motor
void setDirectionOutput(uint8_t value);

#endif