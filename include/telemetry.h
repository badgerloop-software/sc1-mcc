#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "mbed.h"

// Bit field to store digital IO
struct Digital_Data {
  bool cruiseEnabled : 1;
  bool motorPower : 1;
  bool forwardAndReverse : 1;
  bool ecoMode : 1;
};

enum class CRUZ_MODE {
  OFF,
  SPEED,
  POWER,
};


//TODO: Tune values based on testing
#define BRAKE_THRESHOLD_CRUZ_STOP 0.1 // used to go out of Cruise Control. NOT the final value
#define INCREMENT_RPM   21
#define INCREMENT_POWER 420

#define FORWARD_VALUE 1
#define REVERSE_VALUE 0

// Variables to store readings
extern volatile struct Digital_Data digital_data;
extern volatile CRUZ_MODE cruzMode;

extern volatile float rpm;
extern volatile float mph;

extern volatile float acceleratorPedal;
extern volatile float brakeStatus;
extern volatile float regenerativeBraking;

extern volatile float motorSpeedSetpoint;

// Serial signals
extern volatile bool parkBrake;

#endif