#include "analog.h"
#include "can.h"




// Variables
float curAcc = 0;
float curBrk = 0;
float curPedal = 0;
float pedal_percent_pressed = 0;

// Initialize Analog pins
AnalogIn ACC_SIG(A0);
AnalogIn BRK_SIG(A1);
AnalogOut ACC_OUT(A4);
AnalogIn   PEDAL_IN(A5);

// Update function ticker
Ticker ACC_TIMER;


/// Updates analog readings
//  Automatically triggers CAN message
void analogUpdate() {
    // Update internal values
    curAcc = ACC_SIG.read_voltage();
    curBrk = BRK_SIG.read_voltage();
    curPedal = PEDAL_IN.read_voltage();
    pedal_percent_pressed = calculate_pedal_press(curPedal);

    // Pass through acceleration from pedal to motor
    ACC_OUT.write(ACC_SIG.read());

    // Send new values over CAN
    queueFlags.set(ACC_SLOT);
    queueFlags.set(BRK_SLOT);
    queueFlags.set(PEDAL_SLOT);
    queueFlags.set(PEDAL_PERCENT_SLOT);
}


int initAnalog(std::chrono::milliseconds pollRateMS) {
    // Reference for ADC. Should match voltage of AREF pin
    ACC_SIG.set_reference_voltage(3.3);
    BRK_SIG.set_reference_voltage(3.3);
    PEDAL_IN.set_reference_voltage(3.3);

    // Start pdating function
    ACC_TIMER.attach(analogUpdate, pollRateMS);

    return 0;
}


int disableAnalog() {
    // Stop updating function
    ACC_TIMER.detach();

    return 0;    
}

// Convert input voltage to a percent of pedal pressed
float calculate_pedal_press(float voltage){
    float retval = 0;

    // If voltage is greater or equal to full press value, set percentage to 100%
    if(voltage >= PEDAL_FULL_PRESS){
        retval = 1;
    }
    // If voltage is less or equal to  empty press value, set percentage to 0%
    else if(voltage <= PEDAL_NO_PRESS){
        retval = 0;
    }
    else{
        // Scale accordingly
        retval = (voltage - PEDAL_NO_PRESS) / (PEDAL_FULL_PRESS - PEDAL_NO_PRESS);    
    }

    return retval;
}