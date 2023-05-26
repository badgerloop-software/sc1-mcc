#ifndef __MCCSTATE_H__
#define __MCCSTATE_H__
#include "mbed.h"
#include "mcc.h"

// Forward declaration to resolve circular dependency/include
class mccstatemachine;

class MccState
{
public:
	virtual void enter(Mcc* light, MccState& newState) = 0;
	virtual void toggle(Mcc* light) = 0;
	virtual void exit(Mcc* light, MccState& newState) = 0;
	virtual ~MccState() {}
};


#endif // __MCCSTATE_H__