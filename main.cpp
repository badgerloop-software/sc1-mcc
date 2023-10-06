#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"
#include "speed.h"
#include "motor_control.h"

#define SPEED_CALC_INTERVAL 10ms
#define ANALOG_CALC_INTERVAL 10000us
#define DIGITAL_CALC_INTERVAL 10000us

// time between checking for transitions in state machine in us
#define SM_TRANSITION_INTERVAL 1000000us


int main()
{
    // read RPM and MPH every 0.01 second
    startSpeedCalculation(SPEED_CALC_INTERVAL);

    // read all analog inputs every 0.01 second
    initAnalog(ANALOG_CALC_INTERVAL);

    // read all digital inputs every 0.01 second
    initDigital(DIGITAL_CALC_INTERVAL);

    // initialize state machine
    MCCState state_machine(SM_TRANSITION_INTERVAL);

    while (true) {

    }
}
