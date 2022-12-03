#include "../include/gpio.h"
#include "../include/can.h"
#include <chrono>

// Variables 
static uint16_t curGPIO;
static float curRPM;
static uint64_t counter;
static uint8_t rpsCalcConstant;

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
    
}

/// Updates the RPM at a fixed interval
void updateRPS() {
    // Update speed
    curRPM = counter * rpsCalcConstant;
    counter = 0;

    // Send over CAN


}

/// Initializes all GPIO interrupts
//  Initializes functions which update GPIO and rpm at specified interval
//  On update, functions check if change occurs. Queues CAN message if yes
//  Returns 0 on success, -1 on failure
int initGPIO(std::chrono::microseconds pollPeriodMS, std::chrono::microseconds rpmCalcPeriodMS) {
    
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
    

    return 0;
}

