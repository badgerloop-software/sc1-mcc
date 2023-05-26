#include "../include/mcc.h"
#include "../include/mccstatelist.h"

Mcc::Mcc() {
    currentState = &MccOff::getInstance();
}

void Mcc::setState(MccState& newState) {
    currentState->exit(this, newState);
    currentState = &newState;
    currentState->enter(this, newState);
}

void Mcc::toggle() {
    currentState->toggle(this);
}