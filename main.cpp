#include "mbed.h"
#include "telemetry.h"
#include "digital.h"
#include "analog.h"

// Main code. Should initialize devices and then transform into the CAN loop


int main()
{
    // read all analog inputs every 0.01 second
    initAnalog(10000us);

    // read all digital inputs every 0.01 second
    initDigital(10000us);

    while(true){
        
    }
}
