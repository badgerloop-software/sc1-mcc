#include "mbed.h"
#include "analog.h"
#include "can.h"
#include "gpio.h"


// main() runs in its own thread in the OS
int main()
{
    // Initializes devices
    initGPIO(10ms, 100ms);
    initAnalog(100ms);
    initCAN(125000);

    // Enter send loop
    CANSend();
}

