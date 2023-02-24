#include "analog.h"


// Variables
float curAcc = 0;
float curBrk = 0;

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
    curAcc = ACC_SIG.read_voltage();
    curBrk = BRK_SIG.read_voltage();

    // Determine acceleration 
    switch (curState) {
        case 0:
            ACC_OUT.write(0);
            break;
        case 1:
        case 2:
        case 3:
            ACC_OUT.write(curAcc / 3.3);
            break;
        case 4:
            // PID CONTROL SIGNAL GOES HERE
            ACC_OUT.write(0);
            break;
        default:
            ACC_OUT.write(0);
            break;
    }

    // Send new values over CAN
    queueFlags.set(ACC_SLOT);
    queueFlags.set(BRK_SLOT);
}


int initAnalog(std::chrono::milliseconds pollRateMS) {
    // Reference for ADC. Should match voltage of AREF pin
    ACC_SIG.set_reference_voltage(3.3);
    BRK_SIG.set_reference_voltage(3.3);

    // Start pdating function
    ACC_TIMER.attach(analogUpdate, pollRateMS);

    return 0;
}


int disableAnalog() {
    // Stop updating function
    ACC_TIMER.detach();

    return 0;    
}