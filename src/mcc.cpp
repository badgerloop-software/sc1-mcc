#include "../include/mcc.h"
#include "../include/mccstatelist.h"

Mcc::Mcc() {
    // sets initial state to off state 
    currentState = &OFF::getInstance();
}

// change states
void Mcc::setState(MccState& newState) {
    currentState->exit(this, newState); // calls exit method of current state
    currentState = &newState; // switches states
    currentState->enter(this, newState); // calls enter method of next state
}

// Delegate the task of determining the next state to the current state
void Mcc::toggle() {
    currentState->toggle(this);
}