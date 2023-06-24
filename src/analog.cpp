#include "analog.h"

#include "can.h"
#include "common.h"

// Variables
float curAcc                = 0;
float curBrk                = 0;
float curPedal              = 0;
float pedal_percent_pressed = 0;

static float pedal_min      = PEDAL_NO_PRESS;
static float pedal_deadzone = 0.22;

// Initialize Analog pins
// AnalogIn ACC_SIG(A2);
// AnalogIn BRK_SIG(A1);
//AnalogOut BRK_OUT(A4);
AnalogIn PEDAL_IN(PEDAL_IN_PIN);

// Update function ticker
Ticker ACC_TIMER;

float analog_getCurPedal() { return curPedal; }

/// Updates analog readings
//  Automatically triggers CAN message
void analogUpdate()
{
    // Update internal values
    // curAcc = ACC_SIG.read_voltage();
    // curBrk = BRK_SIG.read_voltage();
    float new_curPedal = PEDAL_IN.read_voltage();
#define PEDAL_N (1)
    static float buf[PEDAL_N] = { 0 };
    static int   counter      = 0;
    buf[counter++ % PEDAL_N]  = new_curPedal;
    float avgPedal            = 0;
    for(int i = 0; i < PEDAL_N; i++)
    {
        avgPedal += buf[i];
    }

    curPedal              = avgPedal / PEDAL_N;
    pedal_percent_pressed = calculate_pedal_press(curPedal);

    // Pass through acceleration from pedal to motor
    // ACC_OUT.write(ACC_SIG.read());

    // Send new values over CAN
    queueFlags.set(ACC_SLOT);
    queueFlags.set(BRK_SLOT);
    queueFlags.set(PEDAL_SLOT);
    queueFlags.set(PEDAL_PERCENT_SLOT);
}

int initAnalog(std::chrono::milliseconds pollRateMS)
{
    // Reference for ADC. Should match voltage of AREF pin
    // ACC_SIG.set_reference_voltage(3.3);
    // BRK_SIG.set_reference_voltage(3.3);
    PEDAL_IN.set_reference_voltage(3.3);
  //  BRK_OUT.write(0);
    // Start pdating function
    ACC_TIMER.attach(analogUpdate, pollRateMS);

    return 0;
}

int disableAnalog()
{
    // Stop updating function
    ACC_TIMER.detach();

    return 0;
}

bool calibrate_pedal()
{
    pedal_min = PEDAL_IN.read_voltage();
    return true;
}

// Convert input voltage to a percent of pedal pressed
float calculate_pedal_press(float voltage)
{
    float retval = 0;

    // If voltage is greater or equal to full press value, set percentage to
    // 100%
    if(voltage >= PEDAL_FULL_PRESS)
    {
        retval = 1;
    }
    // If voltage is less or equal to  empty press value, set percentage to 0%
    else if(voltage <= pedal_min + pedal_deadzone)
    {
        retval = 0;
    }
    else
    {
        // Scale accordingly
        retval = (voltage - pedal_min) / (PEDAL_FULL_PRESS - pedal_min);
    }

    return retval;
}