#include "gpio.h"
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

// Variables
uint16_t curGPIO = 0; // Bitmap
float curRPM = 0;
static uint64_t counter = 0;
static float rpsCalcConstant = 0;
uint8_t curState = 0;
float targetValue = 0;

// cruise control variables
static bool existPastValue = false; // for resuming, tracks if a past value exists
static int pastValue = 0;           // tracking previous value for resume

// Bitmap offsets
#define POWER_BIT 0x1 << 0
#define DIRECTION_BIT 0x1 << 1
#define DIRECTION_OUT_BIT 0x1 << 2
#define BRAKE_BIT 0x1 << 3
#define ECO_BIT 0x1 << 4
#define CRZ_EN_BIT 0x1 << 5
#define CRZ_M_BIT 0x1 << 6  // 0 = Mode A, RPM; 1 = Mode B, Power
#define MC_STAT_BIT 0x1 << 7

//the step sizes when incrementing or decrementing speed in cruise control
#define DELTA_POWER 9001
#define DELTA_RPM 10

// Initialize GPIO pins

InterruptIn spdPulse(D1);
InterruptIn MCStatus(D0);
InterruptIn CrzA(D3);
InterruptIn CrzB(D4);
InterruptIn CrzSet(D5);
InterruptIn CrzRsme(D6);
// DigitalOut Power(D7); // 0 Off, 1 On //A2, TODO
DigitalOut DirectionOut(D7);
DigitalOut Direction(D8);   // 0 Fwd, 1 Rev 
// DigitalOut Eco(D9); // 0 Off, 1 On //A3, TODO
InterruptIn PBrkStatus(D9);
InterruptIn Brk2Status(D11);
InterruptIn BrkStatus(D12); // 0 Off, 1 On


// Timers and Function Tickers
Timer GenGPIODebouce;
Ticker RPMTimer;
Ticker GPIOTimer;

/// Clears debounce timer for general GPIO
void clearGenGPIOTimer() { GenGPIODebouce.reset(); }

/// Increments number of ticks to track motor rotation
void incrTick() { counter++; }

// helper function for cruise control (set, resume, brake, etc.)
void CruiseControlLogic(float speed, float pastValue, bool existPastValue, int delta) {
  bool isCruiseEnabled = (curGPIO & CRZ_EN_BIT) > 0; // determines if cruise control is enabled
  
  // if cruise control on, set increments speed and resume decrements speed
  if (isCruiseEnabled && CrzSet.read()) { // set increments speed
    targetValue += delta;
  } else if (isCruiseEnabled && CrzRsme.read()) { // resume decrements speed
    targetValue -= delta;
  }

  // if cruise control is intially off
  if (!isCruiseEnabled && CrzSet.read()) { // if driver presses cruise set
    curGPIO |= CRZ_EN_BIT;
    targetValue = speed;
    pastValue = speed;
    existPastValue = true;

  // if driver presses cruise resume when not enabled
  } else if (!isCruiseEnabled && CrzRsme.read()) { 
    if (existPastValue) { // past value exists, so change only target value
      curGPIO |= CRZ_EN_BIT;
      targetValue = pastValue;

    } 
    // else 
    // { // past value doesn't exist, so set it (Note: currently cut from cruise control)
    //   curGPIO |= CRZ_EN_BIT;
    //   targetValue = speed;
    //   pastValue = speed;
    //   existPastValue = true;
    // }
  }
}

/// Updates the curGPIO
//  Automatically triggers CAN message on change
void updateGPIO() {
  uint16_t oldGPIO = curGPIO;

  // Update readings if signals have been stable
  if (duration_cast<milliseconds>(GenGPIODebouce.elapsed_time()).count() > 50) {
    if (BrkStatus.read()) {
      curGPIO |= BRAKE_BIT;
    } else {
      curGPIO &= ~(BRAKE_BIT);
    }

    if (BrkStatus.read()) { // Stop cruise control if braking
      curGPIO &= ~(CRZ_EN_BIT);
      printf("cruz stop\n");

    } else if (CrzA.read()) { // Mode A selected == RPM, CRZ_M_BIT = 0
      curGPIO &= ~(CRZ_M_BIT);
      CruiseControlLogic(curRPM, pastValue, existPastValue, DELTA_RPM);

    } else if (CrzB.read()) { // Mode B selected == Power, CRZ_M_BIT = 1
      curGPIO |= CRZ_M_BIT;
      //TODO: change curRPM current power draw of motor
      CruiseControlLogic(curRPM, pastValue, existPastValue, DELTA_POWER);

    } else { // If neither mode is selected, turn off Cruise Control
      curGPIO &= ~(CRZ_EN_BIT);
      existPastValue = false;
    }

    if (MCStatus.read()) {
      curGPIO |= MC_STAT_BIT;
    } else {
      curGPIO &= ~(MC_STAT_BIT);
    }
  }

  Eco.write(curGPIO & ECO_BIT);
  // State machine, see README in include for details
  switch (curState) {
  case 0:
    if (curGPIO & POWER_BIT) {
      curState = 1;
      Power.write(1);
    } else {
      Power.write(0);
    }
    break;
  case 1:
    if (!(curGPIO & POWER_BIT)) {
      curState = 0;
    } else if (curRPM < 5) {
      Direction.write(curGPIO & DIRECTION_BIT);
    } else {
      if (Direction.read() == 1) {
        curState = 3;
      } else {
        curState = 2;
      }
    }
    break;
  case 2:
    if (!(curGPIO & POWER_BIT)) {
      curState = 0;
    } else if (curRPM < 5) {
      curState = 1;
    } // TODO: CRZ CNTRL Transition
    break;
  case 3:
    if (!(curGPIO & POWER_BIT)) {
      curState = 0;
    } else if (curRPM < 5) {
      curState = 1;
    } // TODO: CRZ CNTRL Transition
    break;
  case 4: // TODO: CRZ CNTRL
    break;
  default: // BAD, SOMEHOW ENTERED UNKNOWN STATE, SHUTOFF
    curState = 0;
    break;
  }

  // If change, send CAN. Also send if stale in case of drop
  if (oldGPIO != curGPIO ||
      duration_cast<seconds>(GenGPIODebouce.elapsed_time()).count() > 30) {
    queueFlags.set(GPIO_SLOT);
    // Reset timers to avoid overflow
    GenGPIODebouce.reset();
  }
}

/// Updates the RPM calculation
//  Automatically triggers CAN message
void updateRPM() {
  // Update speed calculation
  curRPM = (float)counter * rpsCalcConstant;
  counter = 0;

  // Send over CAN
  queueFlags.set(RPM_SLOT);
}

void initGPIO(milliseconds pollPeriodMS, milliseconds rpmCalcPeriodMS) {
  // Enable interrupts
  spdPulse.rise(incrTick);
  CrzA.rise(clearGenGPIOTimer);
  CrzA.fall(clearGenGPIOTimer);
  CrzB.rise(clearGenGPIOTimer);
  CrzB.fall(clearGenGPIOTimer);
  CrzSet.rise(clearGenGPIOTimer);
  CrzSet.fall(clearGenGPIOTimer);
  CrzRsme.rise(clearGenGPIOTimer);
  CrzRsme.fall(clearGenGPIOTimer);
  MCStatus.rise(clearGenGPIOTimer);
  MCStatus.fall(clearGenGPIOTimer);
  BrkStatus.rise(clearGenGPIOTimer);
  BrkStatus.fall(clearGenGPIOTimer);

  // Initialize Outputs to motor power off, eco off, forward
  Power.write(0);
  Eco.write(0);
  Direction.write(0);

  // Set constant for conversion from motor ticks to rpm
  // Divide by 48 to get rotation, divide by sampling interval to get per
  // second, mult by 60 for min
  rpsCalcConstant = (double)1250 / rpmCalcPeriodMS.count();

  // Start updating functions
  RPMTimer.attach(updateRPM, rpmCalcPeriodMS);
  GPIOTimer.attach(updateGPIO, pollPeriodMS);

  // Start timers
  GenGPIODebouce.start();
}

void disableGPIO() {
  // Disable interrupts
  spdPulse.rise(NULL);
  CrzA.rise(NULL);
  CrzA.fall(NULL);
  CrzB.rise(NULL);
  CrzB.fall(NULL);
  CrzSet.rise(NULL);
  CrzSet.fall(NULL);
  CrzRsme.rise(NULL);
  CrzRsme.fall(NULL);
  MCStatus.rise(NULL);
  MCStatus.fall(NULL);
  BrkStatus.rise(NULL);
  BrkStatus.fall(NULL);

  // Stop updating functions
  RPMTimer.detach();
  GPIOTimer.detach();

  // Set outputs to off
  curState = 0;
  Eco.write(0);
  Direction.write(0);
  Power.write(0);

  // Stop timers
  GenGPIODebouce.stop();
}
