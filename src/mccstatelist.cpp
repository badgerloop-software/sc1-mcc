#include "../include/mccstatelist.h"
#include "../include/common.h"
#include "../include/gpio.h"
#include "../include/drive.h"
#include "../include/analog.h"
// #include <cstdio>

void OFF::enter(Mcc* mcc, MccState& newState) {
    // set cruise control target velocity to 0, set acceleration of motor to 0
    drive_setTargetVelocity(0);
    drive_disableAccel();
}

void OFF::toggle(Mcc* mcc) {}

void OFF::exit(Mcc* mcc, MccState& newState) {}

void OFF::execute() {}

const char* OFF::getStateName() {
    return "OFF";
}

MccState& OFF::getInstance() {
    static OFF singleton;
    return singleton;
}

void PARK::enter(Mcc* mcc, MccState& newState) {
    drive_setTargetVelocity(0);
    drive_disableAccel();
}

void PARK::toggle(Mcc* mcc) {}

void PARK::exit(Mcc* mcc, MccState& newState) {}

void PARK::execute() {}

const char* PARK::getStateName() {
    return "PARK";
}

MccState& PARK::getInstance() {
    static PARK singleton;
    return singleton;
}


void FORWARD::enter(Mcc* mcc, MccState& newState) {
    Direction.write(DIRECTION_FORWARD);
    drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
}

void FORWARD::toggle(Mcc* mcc) {}

void FORWARD::exit(Mcc* mcc, MccState& newState) {

    // checks if going back to Forward state
    if(&newState == &FORWARD::getInstance()) { // prints out an error saying that speed must be 0 before switching to reverse
        printf("Must be at 0 mph before switching to Reverse");
    }
}

void FORWARD::execute() {
    drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
}

const char* FORWARD::getStateName() {
    return "FORWARD";
}

MccState& FORWARD::getInstance() {
    static FORWARD singleton;
    return singleton;
}

void REVERSE::enter(Mcc* mcc, MccState& newState) {
    Direction.write(DIRECTION_REVERSE);
    drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
}

void REVERSE::toggle(Mcc* mcc) {}

void REVERSE::exit(Mcc* mcc, MccState& newState) {

    // checks if going back to Reverse state
    if(&newState == &REVERSE::getInstance()) { // prints out an error saying that speed must be 0 before switching to forward
        printf("Must be at 0 mph before switching to Forward");
    }
}

void REVERSE::execute() {
    drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
}

const char* REVERSE::getStateName() {
    return "REVERSE";
}

MccState& REVERSE::getInstance() {
    static REVERSE singleton;
    return singleton;
}
