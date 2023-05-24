#ifndef __MCCSTATEMACHINE_H__
#define __MCCSTATEMACHINE_H__
#include "mbed.h"
#include "mccstateinterface.h"

class MccState;

class Mcc {
    public:
        Mcc();
        MccState* getCurrentState() const { return currentState; }
        void toggle();
        void setState(MccState& newState);

    private:
        MccState* currentState;
};



#endif // __MCCSTATEMACHINE_H__