#include "mbed.h"
#include "analog.h"
#include "can.h"
#include "gpio.h"


/// Main code. Should initialize devices and then transform into the CAN loop
int main()
{
    // Initializes devices
    initGPIO(1s, 1s);
    initAnalog(1s);
    initCAN(125000);

    // Enter send loop
    CANSend();
}

