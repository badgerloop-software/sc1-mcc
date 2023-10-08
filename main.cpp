#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"
#include "speed.h"

#define SPEED_CALC_INTERVAL 10ms
#define ANALOG_CALC_INTERVAL 10000us
#define DIGITAL_CALC_INTERVAL 10000us


int main()
{
    // read RPM and MPH every 0.01 second
    startSpeedCalculation(SPEED_CALC_INTERVAL);

    // read all analog inputs every 0.01 second
    initAnalog(ANALOG_CALC_INTERVAL);

    // read all digital inputs every 0.01 second
    initDigital(DIGITAL_CALC_INTERVAL);

    while(true) {

    }
}
