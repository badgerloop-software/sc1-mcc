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
#define BRAKE_THRESHOLD 69.0
#define INCREMENT_RPM   21
#define INCREMENT_POWER 420

// Variables to store readings
extern volatile struct Digital_Data digital_data;
extern volatile CRUZ_MODE cruzMode;

extern volatile float rpm;
extern volatile float mph;

extern volatile float acceleratorPedal;
extern volatile float brakeStatus;
extern volatile float regenerativeBraking;

extern volatile float motorSpeedSetpoint;

#endif