#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "mbed.h"

// Bit field to store digital IO
struct Digital_Data {
  bool cruiseEnabled : 1;
  bool motorPower : 1;
  bool forwardAndReverse : 1;
  bool ecoMode : 1;
  bool brakeStatus : 1;
};

enum class CRUZ_MODE : uint8_t {
  OFF,
  SPEED,
  POWER,
};

enum ErrorType : uint8_t {
    None, 
    OverCurrent, 
    HallSensorFault, 
    MotorLocked, 
    SensorFault1, 
    SensorFault2, 
    HighBatteryVoltage, 
    ControllerOverheat, 
    Unknown
};

enum class MCCStates : uint8_t {
    OFF,
    PARK,
    IDLE,
    FORWARD,
    REVERSE,
    CRUISE_POWER,
    CRUISE_SPEED
};

//TODO: Tune values based on testing
#define INCREMENT_RPM   21
#define INCREMENT_POWER 420

// TODO Tune this
#define CRUISE_SPEED_MAX_RPM 1000

#define FORWARD_VALUE 0
#define REVERSE_VALUE 1

// Variables to store readings
extern volatile struct Digital_Data digital_data;
extern volatile CRUZ_MODE cruzMode;
extern volatile ErrorType errorType;
extern volatile MCCStates mccState;

extern volatile float rpm;
extern volatile float mph;

extern volatile float acceleratorPedal;
extern volatile float regenerativeBraking;

extern volatile float motorSpeedSetpoint;

extern volatile bool parkBrake;

extern volatile float speed_pid_compute;

#endif