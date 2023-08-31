#include "analog.h"
#include "can.h"
#include "common.h"
#include "drive.h"
#include "gpio.h"
#include "mbed.h"
#include "monitor.h"

#if defined(BLINK_RATE_250)
#define BLINKING_RATE     250ms
#elif defined(BLINK_RATE)
#define BLINKING_RATE     BLINK_RATE
#else
#define BLINKING_RATE     2000ms
#endif

// Main code. Should initialize devices and then transform into the CAN loop

int main()
{
    int iteration = 0;

#ifdef LED1
    DigitalOut led(LED1);
#else
    bool led;
#endif

    while(1)
    {
        // LED blink
        led = !led;
        printf("Blink num %d\n", iteration);

        // Stripped down MCC
        /*if(!READ_BIT_POS(curGPIO, POWER_BIT))
        {
            drive_setTargetVelocity(0);
            drive_disableAccel();
        }
        else
        {
            drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
        }*/

        // TODO printf("Heyooo %d\n", iteration);
        if(iteration >= 10)
            printf("Done\n");
        // TODO wait_us(SEC_TO_USEC(.75));
        iteration ++;

        // LED blink wait
        ThisThread::sleep_for(BLINKING_RATE);
    }

    // Enter send loop
    // CANSend();
}
