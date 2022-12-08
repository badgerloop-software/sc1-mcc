#include "gpio.h"
#include <chrono>

using namespace std::chrono;

// Variables 
uint16_t curGPIO = 0; // Bitmap
float curRPM = 0;
static uint64_t counter = 0;
static float rpsCalcConstant = 0;

// Bitmap offsets
#define POWER_BIT       0
#define DIRECTON_BIT    1
#define BRAKE_BIT       2
#define ECO_BIT         3
#define CRZ_EN_BIT      4
#define CRZ_M_BIT       5
#define MC_STAT_BIT     6

// Initialize GPIO pins
InterruptIn spdPulse(D1);
InterruptIn CrzA(D3);
InterruptIn CrzB(D4);
InterruptIn CrzSet(D5);
InterruptIn CrzRst(D6);
InterruptIn Power(D0);
InterruptIn Direction(D13);
InterruptIn Eco(D9);
InterruptIn MCStatus(D11);
InterruptIn BrkStatus(D12);
DigitalOut  TestOut(PA_10);

// Timers and Function Tickers
Timer GenGPIODebouce;
Timer BrakeDebounce;
Ticker RPMTimer;
Ticker GPIOTimer;


/// Clears debounce timer for general GPIO
void clearGenGPIOTimer() {
    GenGPIODebouce.reset();
}

/// Clears brake debounce timer
void clearBrakeTimer() {
    BrakeDebounce.reset();
}

/// Increments number of ticks
void incrTick() {
    counter++;
}


/// Updates the curGPIO at fixed interval
//  Automatically triggers CAN message on change
void updateGPIO() {
    uint16_t oldGPIO = curGPIO;

    // Update if signals have been stable
    if (duration_cast<milliseconds>(BrakeDebounce.elapsed_time()).count() > 5) {
        if (BrkStatus.read()) {
            curGPIO |= 1UL << BRAKE_BIT;
        } else {
            curGPIO &= ~(1UL << BRAKE_BIT);
        }
    }

    if (duration_cast<milliseconds>(GenGPIODebouce.elapsed_time()).count() > 5) {
        if (Power.read()) {
            curGPIO |= 1UL << POWER_BIT;
        } else {
            curGPIO &= ~(1UL << POWER_BIT);
        }

        if (Direction.read()) {
            curGPIO |= 1UL << DIRECTON_BIT;
        } else {
            curGPIO &= ~(1UL << DIRECTON_BIT);
        }

        if (Eco.read()) {
            curGPIO |= 1UL << ECO_BIT;
        } else {
            curGPIO &= ~(1UL << ECO_BIT);
        }

        // Cruise off takes priority over on
        if (CrzRst.read()) {
            curGPIO &= ~(1UL << CRZ_EN_BIT);
        } else if (CrzSet.read()) {
            curGPIO |= 1UL << CRZ_EN_BIT;
        }

        // Cruise Mode A priority over B
        if (CrzA.read()) {
            curGPIO &= ~(1UL << CRZ_M_BIT);
        } else if (CrzB.read()) {
            curGPIO |= 1UL << CRZ_M_BIT;
        }

        if (MCStatus.read()) {
            curGPIO |= 1UL << MC_STAT_BIT;
        } else {
            curGPIO &= ~(1UL << MC_STAT_BIT);
        }
    }

    // If change, send CAN
    if (oldGPIO != curGPIO || GenGPIODebouce.elapsed_time() > 1000ms) {
        queueFlags.set(1UL << GPIO_SLOT);
        // Reset timers to avoid overflow
        BrakeDebounce.reset();
        GenGPIODebouce.reset();
    }    
}


/// Updates the RPM at a fixed interval
//  Automatically triggers CAN message
void updateRPS() {
    // Update speed calculation
    curRPM = counter * rpsCalcConstant;
    counter = 0;

    // Send over CAN
    queueFlags.set(1UL << RPM_SLOT);
}


int initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS) {
    // Enable interrupts
    spdPulse.rise(clearGenGPIOTimer);
    spdPulse.fall(clearGenGPIOTimer);
    CrzA.rise(clearGenGPIOTimer);
    CrzA.fall(clearGenGPIOTimer);
    CrzB.rise(clearGenGPIOTimer);
    CrzB.fall(clearGenGPIOTimer);
    CrzSet.rise(clearGenGPIOTimer);
    CrzSet.fall(clearGenGPIOTimer);
    CrzRst.rise(clearGenGPIOTimer);
    CrzRst.fall(clearGenGPIOTimer);
    Power.rise(clearGenGPIOTimer);
    Power.fall(clearGenGPIOTimer);
    Direction.rise(clearGenGPIOTimer);
    Direction.fall(clearGenGPIOTimer);
    Eco.rise(clearGenGPIOTimer);
    Eco.fall(clearGenGPIOTimer);
    MCStatus.rise(clearGenGPIOTimer);
    MCStatus.fall(clearGenGPIOTimer);
    BrkStatus.rise(clearBrakeTimer);
    BrkStatus.fall(clearBrakeTimer);

    // Set constant for conversion from motor ticks to rpm 
    // Divide by 48 to get rotation, divide by sampling interval to get per second, mult by 60 for min
    rpsCalcConstant = 1.25 / rpmCalcPeriodMS.count();
    
    // CAN Queue logistics
    // (char*)(outputQueue[RPM_SLOT].data) = (char*)&curRPM;
    // outputQueue[RPM_SLOT].len = sizeof(float);
    // (char*)(outputQueue[GPIO_SLOT].data) = (char*)&curGPIO;
    // outputQueue[GPIO_SLOT].len = sizeof(uint16_t);

    // Start updating functions
    RPMTimer.attach(updateRPS, rpmCalcPeriodMS);
    GPIOTimer.attach(updateGPIO, pollPeriodMS);

    // Start timers
    GenGPIODebouce.start();
    BrakeDebounce.start();

    TestOut.write(1);

    return 0;
}


int disableGPIO() {
    // Disable interrupts
    spdPulse.rise(NULL);
    spdPulse.fall(NULL);
    CrzA.rise(NULL);
    CrzA.fall(NULL);
    CrzB.rise(NULL);
    CrzB.fall(NULL);
    CrzSet.rise(NULL);
    CrzSet.fall(NULL);
    CrzRst.rise(NULL);
    CrzRst.fall(NULL);
    Power.rise(NULL);
    Power.fall(NULL);
    Direction.rise(NULL);
    Direction.fall(NULL);
    Eco.rise(NULL);
    Eco.fall(NULL);
    MCStatus.rise(NULL);
    MCStatus.fall(NULL);
    BrkStatus.rise(NULL);
    BrkStatus.fall(NULL);

    // Stop updating functions
    RPMTimer.detach();
    GPIOTimer.detach();

    // Stop timers
    GenGPIODebouce.stop();
    BrakeDebounce.stop();

    return 0;
}

