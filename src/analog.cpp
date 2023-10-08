#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

// Disable mutex usage of analog read for interrupt polling
// because mutex cannot be used in ISR
class AnalogInMutexless : public AnalogIn {
public:
    AnalogInMutexless(PinName pin) : AnalogIn(pin) { }
    virtual void lock() { }
    virtual void unlock() { }
};

// assign analog inputs to the correct pins
AnalogInMutexless acceleratorPedalPin(A6);
AnalogInMutexless brakeStatusPin(A1);
AnalogInMutexless regenerativeBrakingPin(A4);

// initialize the analog inputs' readings
volatile float acceleratorPedal = 0;
volatile float brakeStatus = 0;
volatile float regenerativeBraking = 0;

// read accelerator pedal input
void readAcceleratorPedal(){ 
    acceleratorPedal = acceleratorPedalPin.read_voltage();
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
    readBrakeStatus();
    readRegeativeBraking();
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    acceleratorPedalPin.set_reference_voltage(refrence_voltage);
    brakeStatusPin.set_reference_voltage(refrence_voltage);
    regenerativeBrakingPin.set_reference_voltage(refrence_voltage);

    readAnalogDelay.attach(readAnalog, readSignalPeriod);
}
