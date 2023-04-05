#include "mbed.h"
#include "analog.h"
#include "can.h"
#include "gpio.h"
#include "drive.h"
#include "common.h"


/// Main code. Should initialize devices and then transform into the CAN loop
int main()
{
    // Initializes devices
    // initGPIO(1s, 1s);
    initAnalog(1s);
    drive_initDrive();
    // initCAN(125000);

    while(1)
    {       
        printf("\033[2J\033[;H");
        printf("Current Speed: %.2f RPS", drive_getRPS());
        wait_us(SEC_TO_USEC(1.5));
    }

    // Enter send loop
    // CANSend();
}

