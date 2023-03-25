#include "main.h"
#include "mbed.h"

#if MONITOR_MODE
#include "mcc-monitor.h"
#else
#include "analog.h"
#include "can.h"
#include "gpio.h"
#endif // MONITOR_MODE

/// Main code. Should initialize devices and then transform into the CAN loop
int main()
{
#if MONITOR_MODE
runMonitorApp();
#else 
    // Initializes devices
    initGPIO(1s, 1s);
    initAnalog(1s);
    initCAN(125000);

    // Enter send loop
    CANSend();
    #endif // MONITOR_MODE
}

