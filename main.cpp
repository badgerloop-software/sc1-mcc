#include "analog.h"
#include "can.h"
#include "common.h"
#include "drive.h"
#include "gpio.h"
#include "mbed.h"
#include "monitor.h"

// Main code. Should initialize devices and then transform into the CAN loop

int main()
{
    // Initializes devices
    initGPIO(250ms);
    initAnalog(250ms);
    drive_initDrive();
    initCAN(125000);
    monitor_initMonitor(300ms);

    while(1)
    {
        if(!READ_BIT_POS(curGPIO, POWER_BIT))
        {
            drive_setTargetVelocity(0);
            drive_disableAccel();
        }
        else
        {
            drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
        }
        wait_us(SEC_TO_USEC(.75));
    }

    // Enter send loop
    // CANSend();
}
