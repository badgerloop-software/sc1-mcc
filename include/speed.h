#ifndef __SPEED_H__
#define __SPEED_H__

#include "telemetry.h"

// The circumference of the wheel used for mph calculation, in inches
#define WHEEL_CIRCUMFERENCE 73

/*
    Starts speed (RPM and MPH) calculations at specified interval
*/
void startSpeedCalculation(std::chrono::milliseconds interval);


#endif // __SPEED_H__