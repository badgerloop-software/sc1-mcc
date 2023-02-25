#include "gpio.h"
#include <chrono>

using namespace std::chrono;

// Variables 
uint16_t curGPIO = 0; // Bitmap
float curRPM = 0;
static uint64_t counter = 0;
static float rpsCalcConstant = 0;
uint8_t curState = 0;

// Bitmap offsets
#define POWER_BIT       0x1 << 0
#define DIRECTON_BIT    0x1 << 1
#define BRAKE_BIT       0x1 << 2
#define ECO_BIT         0x1 << 3
#define CRZ_EN_BIT      0x1 << 4
#define CRZ_M_BIT       0x1 << 5
#define MC_STAT_BIT     0x1 << 6

// Initialize GPIO pins
DigitalOut Power(D0);       // 0 Off, 1 On
InterruptIn spdPulse(D1);
InterruptIn CrzA(D3);
InterruptIn CrzB(D4);
InterruptIn CrzSet(D5);
InterruptIn CrzRst(D6);
DigitalOut Eco(D9);         // 0 Off, 1 On
InterruptIn MCStatus(D11);
InterruptIn BrkStatus(D12); // 0 Off, 1 On
DigitalOut Direction(D13);  // 0 Fwd, 1 Rev

// Timers and Function Tickers
LowPowerTimer GenGPIODebouce;
Ticker RPMTimer;
Ticker GPIOTimer;


/// Clears debounce timer for general GPIO
void clearGenGPIOTimer() {
    GenGPIODebouce.reset();
}

/// Increments number of ticks
void incrTick() {
    counter++;
}


/// Updates the curGPIO at fixed interval
//  Automatically triggers CAN message on change
void updateGPIO() {
    uint16_t oldGPIO = curGPIO;

    // Update readings if signals have been stable
    if (duration_cast<milliseconds>(GenGPIODebouce.elapsed_time()).count() > 50) {
        if (BrkStatus.read()) {
            curGPIO |= BRAKE_BIT;
        } else {
            curGPIO &= ~(BRAKE_BIT);
        }

        // Cruise off takes priority over on
        if (CrzRst.read()) {
            curGPIO &= ~(CRZ_EN_BIT);
        } else if (CrzSet.read()) {
            curGPIO |= CRZ_EN_BIT;
        }

        // Cruise Mode A priority over B
        if (CrzA.read()) {
            curGPIO &= ~(CRZ_M_BIT);
        } else if (CrzB.read()) {
            curGPIO |= CRZ_M_BIT;
        }

        if (MCStatus.read()) {
            curGPIO |= MC_STAT_BIT;
        } else {
            curGPIO &= ~(MC_STAT_BIT);
        }
    }

    Eco.write(curGPIO & ECO_BIT);
    // State machine
    switch (curState) {
        case 0:
            if (curGPIO & POWER_BIT) {
                curState = 1;
                Power.write(1);
            } else {
                Power.write(0);
            }
            break;
        case 1:
            if (!(curGPIO & POWER_BIT)) {
                curState = 0;
            } else if (curRPM < 5) {
                Direction.write(curGPIO & DIRECTON_BIT);
            } else {
                if (Direction.read() == 1) {
                    curState = 3;
                } else {
                    curState = 2;
                }
            }
            break;
        case 2:
            if (!(curGPIO & POWER_BIT)) {
                curState = 0;
            } else if (curRPM < 5) {
                curState = 1;
            }
            break;
        case 3:
            break;
        default: // BAD, SOMEHOW ENTERED UNKNOWN STATE, SHUTOFF
            curState = 0;
            break;
    }
    

    // If change, send CAN. Also send if stale in case of drop
    if (oldGPIO != curGPIO || duration_cast<seconds>(GenGPIODebouce.elapsed_time()).count() > 30) {
        queueFlags.set(GPIO_SLOT);
        // Reset timers to avoid overflow
        GenGPIODebouce.reset();
    }    
}


/// Updates the RPM at a fixed interval
//  Automatically triggers CAN message
void updateRPM() {
    // Update speed calculation
    curRPM = (float)counter * rpsCalcConstant;
    counter = 0;

    // Send over CAN
    queueFlags.set(RPM_SLOT);
}


int initGPIO(std::chrono::milliseconds pollPeriodMS, std::chrono::milliseconds rpmCalcPeriodMS) {
    // Enable interrupts
    spdPulse.rise(incrTick);
    CrzA.rise(clearGenGPIOTimer);
    CrzA.fall(clearGenGPIOTimer);
    CrzB.rise(clearGenGPIOTimer);
    CrzB.fall(clearGenGPIOTimer);
    CrzSet.rise(clearGenGPIOTimer);
    CrzSet.fall(clearGenGPIOTimer);
    CrzRst.rise(clearGenGPIOTimer);
    CrzRst.fall(clearGenGPIOTimer);
    MCStatus.rise(clearGenGPIOTimer);
    MCStatus.fall(clearGenGPIOTimer);
    BrkStatus.rise(clearGenGPIOTimer);
    BrkStatus.fall(clearGenGPIOTimer);

    // Initialize Outputs to motor power off, eco off, forward
    Power.write(0);
    Eco.write(0);
    Direction.write(0);

    // Set constant for conversion from motor ticks to rpm 
    // Divide by 48 to get rotation, divide by sampling interval to get per second, mult by 60 for min
    rpsCalcConstant = (double)1250 / rpmCalcPeriodMS.count();

    // Start updating functions
    RPMTimer.attach(updateRPM, rpmCalcPeriodMS);
    GPIOTimer.attach(updateGPIO, pollPeriodMS);

    // Start timers
    GenGPIODebouce.start();

    return 0;
}


int disableGPIO() {
    // Disable interrupts
    spdPulse.rise(NULL);
    CrzA.rise(NULL);
    CrzA.fall(NULL);
    CrzB.rise(NULL);
    CrzB.fall(NULL);
    CrzSet.rise(NULL);
    CrzSet.fall(NULL);
    CrzRst.rise(NULL);
    CrzRst.fall(NULL);
    MCStatus.rise(NULL);
    MCStatus.fall(NULL);
    BrkStatus.rise(NULL);
    BrkStatus.fall(NULL);

    // Stop updating functions
    RPMTimer.detach();
    GPIOTimer.detach();

    // Set outputs to off
    curState = 0;
    Eco.write(0);
    Direction.write(0);
    Power.write(0);

    // Stop timers
    GenGPIODebouce.stop();

    return 0;
}

