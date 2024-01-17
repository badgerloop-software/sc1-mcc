#include "digital.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the
// function at a specified rate
Ticker readDigitalDelay;

// assign digital input to the correct pins
DigitalIn setCruiseControlPin(PB_4);
DigitalIn resetCruiseControlPin(PB_5);
DigitalIn cruisePowerModePin(PB_6);
DigitalIn cruiseSpeedModePin(PB_7);
DigitalIn motorPowerPin(PA_0);
DigitalIn directionInputPin(PA_7);
DigitalIn ecoModePin(PA_5);
DigitalIn brakeStatusPin(PA_8);

// assign digital output to the correct pins
DigitalOut directionOutputPin(PA_3);
DigitalOut brakeLEDOutputPin(PF_1);


// storing every digital data inside a struct to reduce the memory
volatile struct Digital_Data digital_data = {};
volatile CRUZ_MODE cruzMode = CRUZ_MODE::OFF;
volatile float motorSpeedSetpoint = 0;


void readCruiseControl() {
  // read set cruise control input
  // increments
  bool setCruz = setCruiseControlPin.read();

  // read reset cruise control input
  // decrment
  bool resetCruz = resetCruiseControlPin.read();

  // read cruise power mode input
  bool cruisePowerMode = cruisePowerModePin.read();

  // read cruise speed mode input
  bool cruiseSpeedMode = cruiseSpeedModePin.read();

  float increment_by = 0;

  // set cruise control mode and set what to increment by
  if (cruisePowerMode) {
    cruzMode = CRUZ_MODE::POWER;
    increment_by = INCREMENT_POWER;
  } else if (cruiseSpeedMode) {
    cruzMode = CRUZ_MODE::SPEED;
    increment_by = INCREMENT_RPM;
  } else {
    cruzMode = CRUZ_MODE::OFF;
  }

  // if mode is off or brake is pressed, cruise control is off (and can't be
  // turn on)
  if (cruzMode == CRUZ_MODE::OFF || digital_data.brakeStatus) {
    digital_data.cruiseEnabled = false;
    return;
  }

  /*---cruise control can be turn on---*/

  // if cruise control is on, make reset and set decrement and increment
  if (digital_data.cruiseEnabled) {
    // increments
    if (setCruz) {
      motorSpeedSetpoint += increment_by;
    }
    // decrement
    if (resetCruz) {
      motorSpeedSetpoint -= increment_by;
    }
  } else {
    // enable cruise control if set cruz or reset is pressed
    if (setCruz) {
      digital_data.cruiseEnabled = true;
      motorSpeedSetpoint = rpm;
    }
    if (resetCruz) {
      digital_data.cruiseEnabled = true;
    }
  }
}


// read motor power input
void readMotorPower() { digital_data.motorPower = motorPowerPin.read(); }

// read forward and reverse input
void readForwardAndReverse() { digital_data.forwardAndReverse = directionInputPin.read(); }

// read eco mode input
void readEcoMode() { digital_data.ecoMode = ecoModePin.read(); }

// read hand/foot brake status
void readBrakeStatus() { digital_data.brakeStatus = brakeStatusPin.read(); }

// read all the digital inputs
void readDigital() {
  readCruiseControl();
  readMotorPower();
  readForwardAndReverse();
  readEcoMode();
  readBrakeStatus();
}

// Set up polling of digital IO at specified rate
void initDigital(std::chrono::microseconds readSignalPeriod) {
  readDigitalDelay.attach(readDigital, readSignalPeriod);
}

// Update value of motor direction output
void setDirectionOutput(uint8_t value) {
    directionOutputPin.write((int)value);
}

// Set value of brake light output
void setBrakeLEDOutput(uint8_t value) {
    brakeLEDOutputPin.write((int)value);
}