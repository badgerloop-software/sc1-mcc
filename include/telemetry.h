#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "mbed.h"

// Bit field to store digital IO
struct Digital_Data{
    bool setCruiseControl:1;
    bool resetCruiseControl:1;
    bool cruisePowerMode:1;
    bool cruiseSpeedMode:1;
    bool speed:1;
    bool motorPower:1;
    bool forwardAndReverse:1;
    bool ecoMode:1;
};

// Variables to store readings
extern volatile struct Digital_Data digital_data;

extern volatile float acceleratorPedal;
extern volatile float brakeStatus;
extern volatile float regenerativeBraking;


#endif