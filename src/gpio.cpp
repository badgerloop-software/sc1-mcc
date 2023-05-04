#include "mbed.h"
#include "gpio.h"
#include "drive.h"
#include "common.h"
#include "analog.h"


#include <chrono>

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
uint16_t curGPIO = 0; // Bitmap
float curRPM = 0;
static uint64_t counter = 0;
static float rpsCalcConstant = 0;



// Initialize GPIO pins
// InterruptIn Power(A4);
// InterruptIn spdPulse(D1);
// InterruptIn CrzA(D3);
// InterruptIn CrzB(D4);
// InterruptIn CrzSet(D5);
// InterruptIn CrzRst(D6);
// InterruptIn Eco(D9);
// InterruptIn MCStatus(D11);
// InterruptIn BrkStatus(D12);
// InterruptIn Direction(D13);
InterruptIn MainTelem(A4);
InterruptIn EcoTelem(A5);
InterruptIn DirTelem(D8);
InterruptIn MCStatus(D0);

// Timers and Function Tickers
BloopTimer GenGPIODebouce;
BloopTimer BrakeDebounce;
Ticker RPMTimer;
Ticker GPIOTimer;


// /// Clears debounce timer for general GPIO
void clearGenGPIOTimer() {
    GenGPIODebouce.reset();
}

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
void updateGPIO() {
    uint16_t oldGPIO = curGPIO;

    // Update if signals have been stable
    // if (duration_cast<milliseconds>(BrakeDebounce.elapsed_time()).count() > 50) {
    //     if (BrkStatus.read()) {
    //         curGPIO |= 1UL << BRAKE_BIT;
    //     } else {
    //         curGPIO &= ~(1UL << BRAKE_BIT);
    //     }
    // }

    if (duration_cast<milliseconds>(GenGPIODebouce.elapsed_time()).count() > 50) {
        if (MainTelem.read()) {
            curGPIO |= 1UL << POWER_BIT;
        } else {
            curGPIO &= ~(1UL << POWER_BIT);
        }

        if (DirTelem.read()) {
            curGPIO |= 1UL << DIRECTON_BIT;
        } else {
            curGPIO &= ~(1UL << DIRECTON_BIT);
        }

        if (EcoTelem.read()) {
            curGPIO |= 1UL << ECO_BIT;
        } else {
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

        // if (MCStatus.read()) {
        //     curGPIO |= 1UL << MC_STAT_BIT;
        // } else {
        //     curGPIO &= ~(1UL << MC_STAT_BIT);
        // }
    }

    // If change, send CAN. Also send if stale in case of drop
    if (oldGPIO != curGPIO || duration_cast<seconds>(GenGPIODebouce.elapsed_time()).count() > 30) {
        queueFlags.set(GPIO_SLOT);
        // Reset timers to avoid overflow
        BrakeDebounce.reset();
        GenGPIODebouce.reset();
    }    
}

void wait3sec(void)
{
    printf("STALLING\n\r");
    calibrate_pedal();
    drive_disableAccel();
    wait_us(SEC_TO_USEC(3));
}


// /// Updates the RPM at a fixed interval
// //  Automatically triggers CAN message
// void updateRPS() {
//     // Update speed calculation
//     curRPM = (float)counter * rpsCalcConstant;
//     counter = 0;

//     // Send over CAN
//     queueFlags.set(RPM_SLOT);
// }


int initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS) {
//     // Enable interrupts
//     spdPulse.rise(incrTick);
//     CrzA.rise(clearGenGPIOTimer);
//     CrzA.fall(clearGenGPIOTimer);
//     CrzB.rise(clearGenGPIOTimer);
//     CrzB.fall(clearGenGPIOTimer);
//     CrzSet.rise(clearGenGPIOTimer);
//     CrzSet.fall(clearGenGPIOTimer);
//     CrzRst.rise(clearGenGPIOTimer);
//     CrzRst.fall(clearGenGPIOTimer);
    // Power.rise(clearGenGPIOTimer);
    // Power.fall(clearGenGPIOTimer);
//     Direction.rise(clearGenGPIOTimer);
//     Direction.fall(clearGenGPIOTimer);
//     Eco.rise(clearGenGPIOTimer);
//     Eco.fall(clearGenGPIOTimer);
//     MCStatus.rise(clearGenGPIOTimer);
//     MCStatus.fall(clearGenGPIOTimer);
//     BrkStatus.rise(clearBrakeTimer);
//     BrkStatus.fall(clearBrakeTimer);
    MainTelem.fall(clearGenGPIOTimer);
    MainTelem.rise(clearGenGPIOTimer);
    EcoTelem.rise(clearGenGPIOTimer);
    EcoTelem.fall(clearGenGPIOTimer);
    DirTelem.rise(clearGenGPIOTimer);
    DirTelem.fall(clearGenGPIOTimer);
    MCStatus.rise(clearGenGPIOTimer);
    MCStatus.fall(clearGenGPIOTimer);
    // INIT_GPIO(MainTelem);
    MainTelem.rise(&wait3sec);
    // INIT_GPIO(EcoTelem);
    // INIT_GPIO(DirTelem);
    // INIT_GPIO(MCStatus);



//     // Set constant for conversion from motor ticks to rpm 
//     // Divide by 48 to get rotation, divide by sampling interval to get per second, mult by 60 for min
//     rpsCalcConstant = (double)1250 / rpmCalcPeriodMS.count();

//     // Start updating functions
    // RPMTimer.attach(updateRPS, rpmCalcPeriodMS);
    GPIOTimer.attach(updateGPIO, pollPeriodMS);

//     // Start timers
    GenGPIODebouce.start();
    BrakeDebounce.start();

    return 0;
}


// int disableGPIO() {
//     // Disable interrupts
//     spdPulse.rise(NULL);
//     CrzA.rise(NULL);
//     CrzA.fall(NULL);
//     CrzB.rise(NULL);
//     CrzB.fall(NULL);
//     CrzSet.rise(NULL);
//     CrzSet.fall(NULL);
//     CrzRst.rise(NULL);
//     CrzRst.fall(NULL);
//     Power.rise(NULL);
//     Power.fall(NULL);
//     Direction.rise(NULL);
//     Direction.fall(NULL);
//     Eco.rise(NULL);
//     Eco.fall(NULL);
//     MCStatus.rise(NULL);
//     MCStatus.fall(NULL);
//     BrkStatus.rise(NULL);
//     BrkStatus.fall(NULL);

//     // Stop updating functions
//     RPMTimer.detach();
//     GPIOTimer.detach();

//     // Stop timers
//     GenGPIODebouce.stop();
//     BrakeDebounce.stop();

//     return 0;
// }

