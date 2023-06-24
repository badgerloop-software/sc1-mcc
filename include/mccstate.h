#ifndef __MCCSTATE_H__
#define __MCCSTATE_H__
#include "mbed.h"
#include "mcc.h"

// Forward declaration to resolve circular dependency/include
class mccstatemachine;

// interface for an Mcc State
class MccState
{
public:
	virtual void enter(Mcc* light, MccState& newState) = 0; // do stuff before changing states
	virtual void toggle(Mcc* light) = 0; // let current state choose next state
	virtual void exit(Mcc* light, MccState& newState) = 0; // do stuff when entering the state
    virtual void execute() = 0; // do stuff when in the state
    virtual const char* getStateName() = 0;
	virtual ~MccState() {} // destructor
};


#endif // __MCCSTATE_H__