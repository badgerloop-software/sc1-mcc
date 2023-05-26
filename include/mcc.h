#ifndef __MCC_H__
#define __MCC_H__
#include "mbed.h"
//#include "mccstate.h"

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



#endif // __MCC_H__