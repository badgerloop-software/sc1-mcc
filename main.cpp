#include "mbed.h"
#include "analog.h"
#include "can.h"
#include "gpio.h"
#include "drive.h"
#include "common.h"


/// Main code. Should initialize devices and then transform into the CAN loop

// InterruptIn main_telem(A4);
// InterruptIn eco_telem(A5);
// InterruptIn dir_telem(D8);
// InterruptIn mc_status(D0);

static float counter = 0;



void ramp_to(float setpoint)
{
    if (setpoint >= drive_getAcc())
    {
        drive_overrideAccel(setpoint);
    }
    else {
        float counter = 0;
        while(setpoint < drive_getAcc())
        {
            drive_overrideAccel(counter += 0.1);
            wait_us(SEC_TO_USEC(0.01));
        }
    }
}

int main()
{
    // Initializes devices
    // initGPIO(1s, 1s);
    initAnalog(250ms);
    drive_initDrive();
    initCAN(125000);

    while(1)
    {
        printf("\033[2J\033[;H");
        if (curGPIO)
        {
            printf("Motor controller is off\n\r");
            drive_setTargetVelocity(0);
            drive_disableAccel();

        }
        else
        {
            #if CRUISE_CONTROL_PEDAL
            float target = 4 * calculate_pedal_press(analog_getCurPedal());
            drive_setTargetVelocity(target);
            #else
            drive_overrideAccel(VOLT_TO_PERCENT(1.5) + calculate_pedal_press(analog_getCurPedal()));
            #endif // CRUISE_CONTROL_PEDAL
            // ramp_to(calculate_pedal_press(analog_getCurPedal()));
            // counter = counter < 3.8 ? (counter + 0.1) : counter;
            // drive_overrideAccel(VOLT_TO_PERCENT(counter));
            // drive_setTargetVelocity(1);
            // float percent = VOLT_TO_PERCENT(3.8);
            // printf("Driving to %.2f\n\r", percent);
            // drive_overrideAccel(percent);
            printf("Current Speed %.2f RPS\n\r", drive_getRPS());
            #if CRUISE_CONTROL_PEDAL
            printf("Targeted RPS  %.2f RPS\n\r", target);
            #endif // CRUISE_CONTROL_PEDAL
            printf("Main Telem: ");
            printf("%d\n\r", curGPIO & (1 << POWER_BIT));
            printf("Eco Telem: %s\n\r", !(curGPIO & (1 << ECO_BIT)) ?  "Power" : "ECO");
            printf("Dir Telem: %s\n\r", (curGPIO & (1 << DIRECTON_BIT)) ? "FWD" : "REV");
            printf("MC Status: %d\n\r", !(curGPIO & (1 << MC_STAT_BIT)));
            printf("Pedal Per: %.2f%\n\r", calculate_pedal_press(analog_getCurPedal()));
            printf("Current Speed: %.2f MPH", RPS_TO_MPH(drive_getRPS()));
        }  
        wait_us(SEC_TO_USEC(.5));
    }
    
    // Enter send loop
    // CANSend();
}

