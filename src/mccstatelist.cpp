#include "../include/mccstatelist.h"
#include "../include/common.h"
#include "../include/gpio.h"
#include "../include/drive.h"
#include "../include/analog.h"
#include <cstdio>

DigitalOut Direction(DIR_OUT_PIN);

// not quite sure if this is right
// believe I still need to add one more transition between Off and Forward/Reverse
void MccOff::enter(Mcc* mcc, MccState& newState) {
    // set cruise control target velocity to 0, set acceleration of motor to 0
    drive_setTargetVelocity(0);
    drive_disableAccel();
}

void MccOff::toggle(Mcc* mcc) {}

void MccOff::exit(Mcc* mcc, MccState& newState) {}

MccState& MccOff::getInstance() {
    static MccOff singleton;
    return singleton;
}

void MccPark::enter(Mcc* mcc, MccState& newState) {
    drive_setTargetVelocity(0); // not sure if this is done in Park State
    drive_disableAccel();
}

void MccPark::toggle(Mcc* mcc) {}

void MccPark::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Park state to Forward state
    if(&newState == &MccForward::getInstance()) {
        Direction.write(DIRECTION_FORWARD); // set direction of motor to forward
        // set acceleration of motor to acceleration of pedal
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal())); 
    }

    else { // checks if going from Park state to Reverse state
        Direction.write(DIRECTION_REVERSE); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }
}

MccState& MccPark::getInstance() {
    static MccPark singleton;
    return singleton;
}


void MccForward::enter(Mcc* mcc, MccState& newState) {}

void MccForward::toggle(Mcc* mcc) {}

void MccForward::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Forward state to Reverse state
    if(&newState == &MccReverse::getInstance()) { 
        Direction.write(DIRECTION_REVERSE); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    // checks if going from Forward state to Cruise state
    else if(&newState == &MccCruise::getInstance()) {

        // do any other cruise stuff, not sure if this is everything
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    // checks if going back to Forward state
    else if(&newState == &MccForward::getInstance()) { // prints out an error saying that speed must be 0 before switching to reverse
        printf("Must be at 0 mph before switching to Reverse");
    }
}

MccState& MccForward::getInstance() {
    static MccForward singleton;
    return singleton;
}

void MccReverse::enter(Mcc* mcc, MccState& newState) {}

void MccReverse::toggle(Mcc* mcc) {}

void MccReverse::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Reverse state to Forward state
    if(&newState == &MccReverse::getInstance()) { 
        Direction.write(DIRECTION_FORWARD); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    // checks if going back to Reverse state
    else if(&newState == &MccReverse::getInstance()) { // prints out an error saying that speed must be 0 before switching to forward
        printf("Must be at 0 mph before switching to Forward");
    }
}

MccState& MccReverse::getInstance() {
    static MccReverse singleton;
    return singleton;
}