#ifndef COMMON_H_
#define COMMON_H_

#include "mbed.h"


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

extern volatile struct Digital_Data digital_data;

extern volatile float acceleratorPedal;
extern volatile float acceleratorSignal;
extern volatile float brakeStatus;
extern volatile float regenerativeBraking;


#endif