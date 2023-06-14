#include "../include/mccstatelist.h"
#include "../include/common.h"
#include "../include/gpio.h"
#include "../include/drive.h"
#include "../include/analog.h"
#include <cstdio>

//DigitalOut Direction(DIR_OUT_PIN);

// not quite sure if this is right
// believe I still need to add one more transition between Off and Forward/Reverse
void OFF::enter(Mcc* mcc, MccState& newState) {
    // set cruise control target velocity to 0, set acceleration of motor to 0
    drive_setTargetVelocity(0);
    drive_disableAccel();
}

void OFF::toggle(Mcc* mcc) {}

void OFF::exit(Mcc* mcc, MccState& newState) {}

MccState& OFF::getInstance() {
    static OFF singleton;
    return singleton;
}

void PARK::enter(Mcc* mcc, MccState& newState) {
    drive_setTargetVelocity(0); // not sure if this is done in Park State
    drive_disableAccel();
}

void PARK::toggle(Mcc* mcc) {}

void PARK::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Park state to Forward state
    if(&newState == &FORWARD::getInstance()) {
        Direction.write(DIRECTION_FORWARD); // set direction of motor to forward
        // set acceleration of motor to acceleration of pedal
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal())); 
    }

    else { // checks if going from Park state to Reverse state
        Direction.write(DIRECTION_REVERSE); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }
}

MccState& PARK::getInstance() {
    static PARK singleton;
    return singleton;
}


void FORWARD::enter(Mcc* mcc, MccState& newState) {}

void FORWARD::toggle(Mcc* mcc) {}

void FORWARD::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Forward state to Reverse state
    if(&newState == &REVERSE::getInstance()) { 
        Direction.write(DIRECTION_REVERSE); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    // checks if going back to Forward state
    else if(&newState == &FORWARD::getInstance()) { // prints out an error saying that speed must be 0 before switching to reverse
        printf("Must be at 0 mph before switching to Reverse");
    }
}

MccState& FORWARD::getInstance() {
    static FORWARD singleton;
    return singleton;
}

void REVERSE::enter(Mcc* mcc, MccState& newState) {}

void REVERSE::toggle(Mcc* mcc) {}

void REVERSE::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Reverse state to Forward state
    if(&newState == &REVERSE::getInstance()) { 
        Direction.write(DIRECTION_FORWARD); // set direction of motor to reverse
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    // checks if going back to Reverse state
    else if(&newState == &REVERSE::getInstance()) { // prints out an error saying that speed must be 0 before switching to forward
        printf("Must be at 0 mph before switching to Forward");
    }
}

MccState& REVERSE::getInstance() {
    static REVERSE singleton;
    return singleton;
}