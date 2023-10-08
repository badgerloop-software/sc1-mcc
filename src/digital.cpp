#include "digital.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readDigitalDelay;

// assign digital input to the correct pins
DigitalIn setCruiseControlPin(D5);
DigitalIn resetCruiseControlPin(D6);
DigitalIn cruisePowerModePin(D3);
DigitalIn cruiseSpeedModePin(D4);
DigitalIn motorPowerPin(D12);
DigitalIn forwardAndReversePin(D8);
DigitalIn ecoModePin(D11);


// storing every digital data inside a struct to reduce the memory
volatile struct Digital_Data digital_data = {};


// read set cruise control input
void readSetCruiseControl(){
    digital_data.setCruiseControl = setCruiseControlPin.read();
}

// read reset cruise control input
void readResetCruiseControl(){
    digital_data.resetCruiseControl = resetCruiseControlPin.read();
}

// read cruise power mode input
void readCruisePowerMode(){
    digital_data.cruisePowerMode = cruisePowerModePin.read();
}

// read cruise speed mode input
void readCruiseSpeedMode(){
    digital_data.cruiseSpeedMode = cruiseSpeedModePin.read();
}

// read motor power input
void readMotorPower(){
    digital_data.motorPower = motorPowerPin.read();
}

// read forward and reverse input
void readForwardAndReverse(){
    digital_data.forwardAndReverse = forwardAndReversePin.read();
}

// read eco mode input
void readEcoMode(){
    digital_data.ecoMode = ecoModePin.read();
}

// read all the digital inputs
void readDigital(){
    readSetCruiseControl();
    readResetCruiseControl();
    readCruisePowerMode();
    readCruiseSpeedMode();
    readMotorPower();
    readForwardAndReverse();
    readEcoMode();
}

// Set up polling of digital IO at specified rate
void initDigital(std::chrono::microseconds readSignalPeriod) {
  readDigitalDelay.attach(readDigital, readSignalPeriod);
}