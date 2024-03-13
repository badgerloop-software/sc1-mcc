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

// TODO: Tune values based on testing
#define INCREMENT_RPM   21
#define INCREMENT_POWER 420

// TODO Tune this
#define CRUISE_SPEED_MAX_RPM 1000

#define FORWARD_VALUE 0
#define REVERSE_VALUE 1

/////////////////////
// analog.h macros //
/////////////////////
// defining the reference voltage for analog inputs
#define REFERENCE_VOLTAGE 3.3

#define PEDAL_ON_THRESHOLD 0.25 // 1.5V/5V
#define PEDAL_RANGE 0.417  // 0.86 - 0.3, where 0.86 = 4.33V/5V is the max

#define REGEN_ON_THRESHOLD 0 // TODO set actual value
#define REGEN_RANGE 1 // TODO set actual value

#define BRAKE_SENSOR_THRESHOLD 0.14 // 0.7/5

//////////////////////////
// motor_error.h macros //
//////////////////////////
// The minimum break between two consecutive sequences of pulses
#define SEQUENCE_DELAY 1000ms

// The time without pulses after which the error type will be reset as None
#define RESET_DELAY 3000ms

////////////////////////////
// motor_control.h macros //
////////////////////////////
#define MIN_MOVING_SPEED 3.0        // speed threshold for idle state

// PID macros
#define POWER_P_PARAM 0.0
#define POWER_I_PARAM 0.0
#define POWER_D_PARAM 0.0
#define SPEED_P_PARAM 2.5
#define SPEED_I_PARAM 0.0
#define SPEED_D_PARAM 0.0

// limits for power and speed PIDs.
#define MIN_POWER 0.0
#define MAX_POWER 1.0
#define MIN_RPM 0.0
#define MAX_RPM 1000.0

// limits for outputs of PID
// is 0.0 to 1.0 due to how AnalogOut pins work.
#define MIN_OUT 0.0
#define MAX_OUT 1.0

////////////////////
// speed.h macros //
////////////////////
// The circumference of the wheel used for mph calculation, in inches
#define WHEEL_CIRCUMFERENCE 73


// Variables to store readings
extern volatile struct Digital_Data digital_data;
extern volatile CRUZ_MODE cruzMode;
extern volatile ErrorType errorType;
extern volatile MCCStates mccState;

extern volatile float rpm;
extern volatile float mph;

extern volatile float acceleratorPedal;
extern volatile float regenerativeBraking;
extern volatile float brakeSensor;

extern volatile float motorSpeedSetpoint;

extern volatile bool parkBrake;

extern volatile float speed_pid_compute;

#endif