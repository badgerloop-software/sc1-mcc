#include "gpio.h"
#include <chrono>

// Variables 
static uint16_t curGPIO;        // See header for bitmap format
static float curRPM; 
static uint64_t counter;
static float rpsCalcConstant;

// Initialize GPIO pins
InterruptIn spdPulse(D1);
InterruptIn CrzA(D3);
InterruptIn CrzB(D4);
InterruptIn CrzSet(D5);
InterruptIn CrzRst(D6);
InterruptIn Power(D7);
InterruptIn Direction(D8);
InterruptIn Eco(D9);
InterruptIn MCStatus(D11);
InterruptIn BrkStatus(D12);

// Timers and Function Tickers
LowPowerTimer GenGPIODebouce;
LowPowerTimer BrakeDebounce;

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
void updateGPIO() {
    // Brake
    if (BrakeDebounce.elapsed_time() > 5ms) {

    }
    if (GenGPIODebouce.elapsed_time() > 5ms) {

    }

    BrakeDebounce.reset();
    GenGPIODebouce.reset();
}

/// Updates the RPM at a fixed interval
void updateRPS() {
    // Update speed
    curRPM = counter * rpsCalcConstant;
    counter = 0;

    // Send over CAN
    if (queueSem.try_acquire_for(1ms)) {
        *(outputQueue[0].data) = curRPM;

        queueSem.release();
        queueFlags.set(1);
    }
}

/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and rpm at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS) {
    
    // Interrupts
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
    
    // Updating functions
    RPMTimer.attach(updateRPS, rpmCalcPeriodMS);
    GPIOTimer.attach(updateGPIO, pollPeriodMS);

    // Start timers
    GenGPIODebouce.start();
    BrakeDebounce.start();

    return 0;
}

/// Disables all GPIO interrupts and update functions
//  Run and reinit if desired to change poll/rps periods
//  Returns 0 on success, -1 on failure
int disableGPIO() {
    // Interrupts
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

    // Updating functions
    RPMTimer.detach();
    GPIOTimer.detach();

    // Start timers
    GenGPIODebouce.stop();
    BrakeDebounce.stop();
    return 0;
}

