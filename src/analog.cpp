#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

// assign analog inputs to the correct pins
AnalogInMutexless acceleratorPedalPin(A6);
AnalogInMutexless brakeStatusPin(A1);
AnalogInMutexless regenerativeBrakingPin(A4);

// assign analog outputs to the correct pins
//AnalogOutMutexless motorAccelerationOutput(A3);
PwmOut motorAccelerationOutput(A3);

// initialize the analog inputs' readings
volatile float acceleratorPedal = 0;
volatile float brakeStatus = 0;
volatile float regenerativeBraking = 0;


// read accelerator pedal input
void readAcceleratorPedal(){
    float value = (acceleratorPedalPin.read() - PEDAL_ON_THRESHOLD) / PEDAL_RANGE;
    if (value < 0) {
        acceleratorPedal = 0;
    } else if (value > 1) {
        acceleratorPedal = 1;
    } else {
        acceleratorPedal = value;
    }    
}

// read brake status input
void readBrakeStatus(){
    brakeStatus = brakeStatusPin.read_voltage();
}

// read regeative braking input
void readRegenerativeBraking(){
    regenerativeBraking = regenerativeBrakingPin.read_voltage();
}

// read all analog input
void readAnalog(){
    readAcceleratorPedal();
    readBrakeStatus();
    readRegenerativeBraking();
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    acceleratorPedalPin.set_reference_voltage(refrence_voltage);
    brakeStatusPin.set_reference_voltage(refrence_voltage);
    regenerativeBrakingPin.set_reference_voltage(refrence_voltage);

    readAnalogDelay.attach(readAnalog, readSignalPeriod);
}

// set the value of the acc_out pin
void setAccOut(float acc) {
    motorAccelerationOutput.write(1-acc);
}