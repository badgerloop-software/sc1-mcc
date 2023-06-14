#include "gpio.h"

#include <chrono>

#include "analog.h"
#include "common.h"
#include "drive.h"
#include "mbed.h"

// #define STUPID_TIMER
// void clearGenGPIOTimer();
// #define INIT_GPIO(x) (x.rise(clearGenGPIOTimer); x.fall(clearGenGPIOTimer);)

#ifdef STUPID_TIMER
typedef LowPowerTimer BloopTimer;
#else
typedef Timer BloopTimer;
#endif

using namespace std::chrono;

// Variables
uint16_t        curGPIO         = 0; // Bitmap
float           curRPM          = 0;
static uint64_t counter         = 0;
static float    rpsCalcConstant = 0;

// Initialize GPIO pins
InterruptIn MainTelem(MAIN_TELEM_PIN);
InterruptIn EcoTelem(ECO_TELEM_PIN);
InterruptIn DirTelem(DIR_TELEM_PIN);

// Timers and Function Tickers
BloopTimer GenGPIODebouce;
BloopTimer BrakeDebounce;
Ticker     RPMTimer;
Ticker     GPIOTimer;

// /// Clears debounce timer for general GPIO
void clearGenGPIOTimer() { GenGPIODebouce.reset(); }

// /// Clears brake debounce timer
// void clearBrakeTimer() {
//     BrakeDebounce.reset();
// }

// /// Increments number of ticks
// void incrTick() {
//     counter++;
// }

// /// Updates the curGPIO at fixed interval
// //  Automatically triggers CAN message on change
void updateGPIO()
{
    uint16_t oldGPIO = curGPIO;

    // Update if signals have been stable
    // if (duration_cast<milliseconds>(BrakeDebounce.elapsed_time()).count() >
    // 50)
    // {
    //     if (BrkStatus.read()) {
    //         curGPIO |= 1UL << BRAKE_BIT;
    //     } else {
    //         curGPIO &= ~(1UL << BRAKE_BIT);
    //     }
    // }

    if(duration_cast<milliseconds>(GenGPIODebouce.elapsed_time()).count() > 50)
    {
        if(MainTelem.read())
        {
            curGPIO |= 1UL << POWER_BIT;
        }
        else
        {
            curGPIO &= ~(1UL << POWER_BIT);
        }

        if(DirTelem.read())
        {
            curGPIO |= 1UL << DIRECTON_BIT;
        }
        else
        {
            curGPIO &= ~(1UL << DIRECTON_BIT);
        }

        if(EcoTelem.read())
        {
            curGPIO |= 1UL << ECO_BIT;
        }
        else
        {
            curGPIO &= ~(1UL << ECO_BIT);
        }

        // // Cruise off takes priority over on
        // if (CrzRst.read()) {
        //     curGPIO &= ~(1UL << CRZ_EN_BIT);
        // } else if (CrzSet.read()) {
        //     curGPIO |= 1UL << CRZ_EN_BIT;
        // }

        // // Cruise Mode A priority over B
        // if (CrzA.read()) {
        //     curGPIO &= ~(1UL << CRZ_M_BIT);
        // } else if (CrzB.read()) {
        //     curGPIO |= 1UL << CRZ_M_BIT;
        // }
    }
    // printf("%x\n\r", curGPIO);
    // If change, send CAN. Also send if stale in case of drop
    if(oldGPIO != curGPIO
       || duration_cast<seconds>(GenGPIODebouce.elapsed_time()).count() > 30)
    {
        queueFlags.set(GPIO_SLOT);
        // Reset timers to avoid overflow
        BrakeDebounce.reset();
        GenGPIODebouce.reset();
    }
}

/**
 * Per Motor Controller Spec the accel signal needs to be set to 0 for 3
 * seconds after enabling the motor controller. This function will stall
 * the car for 3 seconds.
 */
void wait3sec(void)
{
    printf("STALLING\n\r");
    calibrate_pedal();
    drive_disableAccel();
    wait_us(SEC_TO_USEC(3));
}

int initGPIO(std::chrono::milliseconds pollPeriodMS)
{
    //     // Enable interrupts
    MainTelem.fall(clearGenGPIOTimer);
    MainTelem.rise(clearGenGPIOTimer);
    EcoTelem.rise(clearGenGPIOTimer);
    EcoTelem.fall(clearGenGPIOTimer);
    DirTelem.rise(clearGenGPIOTimer);
    DirTelem.fall(clearGenGPIOTimer);
    MainTelem.rise(&wait3sec);

    GPIOTimer.attach(updateGPIO, pollPeriodMS);

    // Start timers
    GenGPIODebouce.start();
    BrakeDebounce.start();

    return 0;
}

/**
 * Not currently used but good to think about
 */
int disableGPIO()
{
    // Disable interrupts
    MainTelem.fall(NULL);
    MainTelem.rise(NULL);
    EcoTelem.rise(NULL);
    EcoTelem.fall(NULL);
    DirTelem.rise(NULL);
    DirTelem.fall(NULL);
    MainTelem.rise(NULL);

    // Stop updating functions
    RPMTimer.detach();
    GPIOTimer.detach();

    // Stop timers
    GenGPIODebouce.stop();
    BrakeDebounce.stop();

    return 0;
}
