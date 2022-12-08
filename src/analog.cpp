#include "analog.h"


// Variables
float acc_in = 0;
float brk_in = 0;

// Initialize Analog pins
AnalogIn ACC_SIG(A0);
AnalogIn BRK_SIG(A1);
AnalogOut ACC_OUT(A4);

// Update function ticker
Ticker ACC_TIMER;


/// Updates analog readings
//  Automatically triggers CAN message
void analogUpdate() {
    // Update internal values
    acc_in = ACC_SIG.read_voltage();
    brk_in = BRK_SIG.read_voltage();

    // Pass through acceleration from pedal to motor
    ACC_OUT.write(acc_in);

    // Send new values over CAN
    queueFlags.set(1UL << ACC_SLOT);
    queueFlags.set(1UL << BRK_SLOT);
}


int initAnalog(std::chrono::milliseconds pollRateMS) {
    // Reference for ADC. Should match voltage of AREF pin
    ACC_SIG.set_reference_voltage(3.3);
    BRK_SIG.set_reference_voltage(3.3);

    // CAN Queue logistics
    // *(char**)(&(outputQueue[ACC_SLOT].data)) = (char*)&acc_in;
    // outputQueue[ACC_SLOT].len = sizeof(float);
    // *(char**)(&(outputQueue[BRK_SLOT].data)) = (char*)&brk_in;
    // outputQueue[BRK_SLOT].len = sizeof(float);

    // Start pdating function
    ACC_TIMER.attach(analogUpdate, pollRateMS);

    return 0;
}


int disableAnalog() {
    // Stop updating function
    ACC_TIMER.detach();

    return 0;    
}