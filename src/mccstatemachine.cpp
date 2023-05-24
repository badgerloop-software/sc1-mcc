#include "mccstatemachine.h"
#include "concretemccstates.h"

Mcc::Mcc() {
    currentState = &MccOff::getInstance();
}

void Mcc::setState(LightState& newState) {
    currentState->exit(this);
    currentState = &newState;
    currentState->enter(this);
}

void Mcc::toggle() {
    currentState->toggle(this);
}