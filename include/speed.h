#ifndef __SPEED_H__
#define __SPEED_H__

#include "telemetry.h"

/*
    Starts speed (RPM and MPH) calculations at specified interval
*/
void startSpeedCalculation(std::chrono::milliseconds interval);


#endif // __SPEED_H__