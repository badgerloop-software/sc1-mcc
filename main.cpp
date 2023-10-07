#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"

// Main code. Should initialize devices and then transform into the CAN loop


int main()
{
    // read all analog inputs every 0.01 second
    initAnalog((std::chrono::microseconds)10000);

    // read all digital inputs every 0.01 second
    initDigital((std::chrono::microseconds)10000);

    while(true){

    }
}
