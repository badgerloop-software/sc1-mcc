#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

// assign analog inputs to the correct pins
AnalogIn acceleratorPedalPin(A6);
AnalogIn acceleratorSignalPin(A3);
AnalogIn brakeStatusPin(A1);
AnalogIn regenerativeBrakingPin(A4);

// initialize the analog inputs' readings
volatile float acceleratorPedal = 0;
volatile float acceleratorSignal = 0;
volatile float brakeStatus = 0;
volatile float regenerativeBraking = 0;

// read accelerator pedal input
void readAcceleratorPedal(){ 
    acceleratorPedal = acceleratorPedalPin.read_voltage();
}

// read accelerator signal input
void readAcceleratorSignal(){
    acceleratorSignal = acceleratorSignalPin.read_voltage();
}

// read brake status input
void readBrakeStatus(){
    brakeStatus = brakeStatusPin.read_voltage();
}

// read regeative braking input
void readRegeativeBraking(){
    regenerativeBraking = regenerativeBrakingPin.read_voltage();
}

// read all analog input
void readAnalog(){
    readAcceleratorPedal();
    readAcceleratorSignal();
    readBrakeStatus();
    readRegeativeBraking();
}

// Poll analog inputs at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {

    acceleratorPedalPin.set_reference_voltage(refrence_voltage);
    acceleratorSignalPin.set_reference_voltage(refrence_voltage);
    brakeStatusPin.set_reference_voltage(refrence_voltage);
    regenerativeBrakingPin.set_reference_voltage(refrence_voltage);

    readAnalogDelay.attach(&readAnalog, readSignalPeriod);
}
