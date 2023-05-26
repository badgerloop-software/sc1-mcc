#include "../include/mccstatelist.h"
#include "../include/common.h"
#include "../include/gpio.h"
#include "../include/drive.h"
#include "../include/analog.h"

DigitalOut Direction(DIR_OUT_PIN);

void MccOff::enter(Mcc* mcc, MccState& newState) {
    drive_setTargetVelocity(0);
    drive_disableAccel();
}

void MccOff::toggle(Mcc* mcc) {
    
}

void MccOff::exit(Mcc* mcc, MccState& newState) {

}

MccState& MccOff::getInstance() {
    static MccOff singleton;
    return singleton;
}

void MccPark::enter(Mcc* mcc, MccState& newState) {

}

void MccPark::toggle(Mcc* mcc) {
    
}

void MccPark::exit(Mcc* mcc, MccState& newState) {

    // checks if going from Park state to Forward state
    if(&newState == &MccForward::getInstance()) {
        Direction.write(DIRECTION_FORWARD);
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }

    else { // checks if going from Park state to Reverse state
        Direction.write(DIRECTION_REVERSE);
        drive_overrideAccel(calculate_pedal_press(analog_getCurPedal()));
    }
}

MccState& MccPark::getInstance() {
    static MccPark singleton;
    return singleton;
}