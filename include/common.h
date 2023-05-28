#ifndef __COMMON_H__
#define __COMMON_H__
#include "mbed.h"

// FEATURE FLAGS
// Turn on the functionality to run cruise control PID
#define ENABLE_CRUISE_CONTROL (1)
// Turn on the functionality to use the pedal as a cruise control setpoint -
// useful for testing
#define CRUISE_CONTROL_PEDAL (0)

#define CRUISE_CONTROL_ENABLED (ENABLE_CRUISE_CONTROL || CRUISE_CONTROL_PEDAL)

// DRIVE
#define ACCEL_OP_AMP_BIAS_V (1.5)
#define LOGIC_LEVEL_V       (3.3)

// GPIO
// Bitmap offsets
#define POWER_BIT    0
#define DIRECTON_BIT 1
#define BRAKE_BIT    2
#define ECO_BIT      3
#define CRZ_EN_BIT   4
#define CRZ_M_BIT    5
#define MC_STAT_BIT  6

// PINMAPS
#define MC_STATUS_PIN   (D0)
#define SPEED_PULSE_PIN (D1)
#define DIR_OUT_PIN     (D7)
#define DIR_TELEM_PIN   (D8)

#define ACC_OUT_PIN    (A3)
#define MAIN_TELEM_PIN (D12)
#define ECO_TELEM_PIN  (D11)
#define PEDAL_IN_PIN   (A6)
#define BRAKE_IN_PIN   (A1)
#define BRAKE_OUT      (A4

// curGPIO BIT MEANINGS 
#define DIRECTION_FORWARD (0)
#define DIRECTION_REVERSE (1)
#define PARK_ON           (1)
#define PARK_OFF          (1)
#define POWER_ON          (1)
#define POWER_OFF         (0)
#define CRZ_ON            (1)
#define CRZ_OFF           (0)

#define CAN_RD_PIN (PA_11)
#define CAN_TD_PIN (PA_12)

// BASIC UTILS
#define SEC_TO_USEC(x)  (x * 1e6)
#define MSEC_TO_USEC(x) (x * 1000)
#define USEC_TO_MSEC(x) (x * 0.001)
#define USEC_TO_SEC(x)  (x * 1e-6)

#define VOLT_TO_PERCENT(x) ((x / ACCEL_OP_AMP_BIAS_V) / LOGIC_LEVEL_V)
#define READ_BIT_POS(x, n) ((x >> n) & 0x1)

#define CLEAR_SCREEN_STR "\033[2J\033[;H"

#endif // __COMMON_H__