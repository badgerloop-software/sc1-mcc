#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

// assign analog inputs to the correct pins
AnalogInMutexless acceleratorPedalPin(PB_0);
AnalogInMutexless regenerativeBrakingInputPin(PB_1);
AnalogInMutexless brakeSensorPin(PA_7);

// assign analog outputs to the correct pins
AnalogOutMutexless motorAccelerationOutput(PA_4);
AnalogOutMutexless regenerativeBrakingOutputPin(PA_6);

// initialize the analog inputs' readings
volatile float acceleratorPedal = 0;
volatile float regenerativeBraking = 0;
volatile float brakeSensor = 0;

// set the value of the acc_out pin
void setAccOut(float acc) {
    motorAccelerationOutput.write(acc);
}

// read all analog input
void readAnalog(){
    // read Accelerator Input
    acceleratorPedal = (acceleratorPedalPin.read() - PEDAL_ON_THRESHOLD) / PEDAL_RANGE;
    if (acceleratorPedal < 0) {
        acceleratorPedal = 0;
    } else if (acceleratorPedal > 1) {
        acceleratorPedal = 1;
    } 

    // read regen brake input
    regenerativeBraking = (regenerativeBrakingInputPin.read() - REGEN_ON_THRESHOLD) / REGEN_RANGE;
    if (regenerativeBraking < 0) {
        regenerativeBraking = 0;
    } else if (regenerativeBraking > 1) {
        regenerativeBraking = 1;
    } 

    // read brake sensor
    brakeSensor = brakeSensorPin.read();

    // TODO: update this when regen braking is on the car
    regenerativeBrakingOutputPin.write(0);
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    acceleratorPedalPin.set_reference_voltage(REFERENCE_VOLTAGE);
    regenerativeBrakingInputPin.set_reference_voltage(REFERENCE_VOLTAGE);
    brakeSensorPin.set_reference_voltage(REFERENCE_VOLTAGE);

    readAnalogDelay.attach(readAnalog, readSignalPeriod);
}

