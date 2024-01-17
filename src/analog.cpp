#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

// assign analog inputs to the correct pins
AnalogInMutexless acceleratorPedalPin(PB_0);
AnalogInMutexless regenerativeBrakingInputPin(PB_1);

// assign analog outputs to the correct pins
//AnalogOutMutexless motorAccelerationOutput(A3);
PwmOut motorAccelerationOutput(PA_4);
AnalogOutMutexless regenerativeBrakingOutputPin(PA_6);

// initialize the analog inputs' readings
volatile float acceleratorPedal = 0;
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


// read regeative braking input
void readRegenerativeBrakingInput(){
    regenerativeBraking = regenerativeBrakingInputPin.read_voltage();
}

// read all analog input
void readAnalog(){
    readAcceleratorPedal();
    readRegenerativeBrakingInput();
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    acceleratorPedalPin.set_reference_voltage(refrence_voltage);
    regenerativeBrakingInputPin.set_reference_voltage(refrence_voltage);

    readAnalogDelay.attach(readAnalog, readSignalPeriod);
}

// set the value of the acc_out pin
void setAccOut(float acc) {
    motorAccelerationOutput.write(1-acc);
}

// set the value of the regenerative brake output pin
void setRegenBrakeOut(float value) {
    regenerativeBrakingOutputPin.write(value);
}