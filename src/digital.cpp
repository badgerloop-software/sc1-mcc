#include "digital.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the
// function at a specified rate
Ticker readDigitalDelay;

// assign digital input to the correct pins
DigitalIn setCruiseControlPin(PB_4);
DigitalIn resetCruiseControlPin(PB_5);
DigitalIn parkBrakePin(PB_6); // used to be cruisePowerModePin
DigitalIn cruiseSpeedModePin(PB_7);
DigitalIn motorPowerPin(PA_0);
DigitalIn directionInputPin(PA_9);
DigitalIn ecoModePin(PA_5);

// assign digital output to the correct pins
DigitalOut directionOutputPin(PA_3);
DigitalOut brakeLEDOutputPin(PF_1);


// storing every digital data inside a struct to reduce the memory
volatile struct Digital_Data digital_data = {};
volatile CRUZ_MODE cruzMode = CRUZ_MODE::OFF;
volatile float motorSpeedSetpoint = 0;

volatile bool parkBrake = true;

bool pastSetCruz = false;
bool pastResetCruz = false;


void readCruiseControl() {
  // read set cruise control input
  // increments
  bool setCruz = setCruiseControlPin.read();

  // read reset cruise control input
  // decrement
  bool resetCruz = resetCruiseControlPin.read();

  // read cruise speed mode input
  bool cruiseSpeedMode = cruiseSpeedModePin.read();
  
  float increment_by = 0;

  // set cruise control mode and set what to increment by
  if (cruiseSpeedMode) {
    cruzMode = CRUZ_MODE::SPEED;
    increment_by = INCREMENT_RPM;
  } else {
    cruzMode = CRUZ_MODE::OFF;
  }

  // if mode is off or brake is pressed, cruise control is off (and can't be
  // turn on)
  if (cruzMode == CRUZ_MODE::OFF || digital_data.brakeStatus) {
    digital_data.cruiseEnabled = false;
    cruzMode = CRUZ_MODE::OFF; // make State Machine go back to FORWARD
    return;
  }

  /*---cruise control can be turn on---*/

  // if cruise control is on, make reset and set decrement and increment
  if (digital_data.cruiseEnabled) {
    // increments
    // 2/4: added rising edge detection and limits on how big/small motorSpeedSetpoint can be
    if (setCruz && !pastSetCruz && motorSpeedSetpoint + increment_by <= CRUISE_SPEED_MAX_RPM) {
      motorSpeedSetpoint += increment_by;
    }
    // decrement
    if (resetCruz && !pastResetCruz && motorSpeedSetpoint - increment_by >= 0) {
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

  // update past values used for rising edge detection
  pastSetCruz = setCruz;
  pastResetCruz = resetCruz;
}


// read all the digital inputs
void readDigital() {
  readCruiseControl();
  digital_data.motorPower = motorPowerPin.read();
  digital_data.forwardAndReverse = directionInputPin.read(); 
  digital_data.ecoMode = ecoModePin.read();
  parkBrake = parkBrakePin.read();
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