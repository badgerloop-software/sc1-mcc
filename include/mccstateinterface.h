#ifndef __MCCSTATEINTERFACE_H__
#define __MCCSTATEINTERFACE_H__
#include "mbed.h"
#include "mccstatemachine.h"

// Forward declaration to resolve circular dependency/include
class mccstatemachine;

class MccState
{
public:
	virtual void enter(Mcc* light) = 0;
	virtual void toggle(Mcc* light) = 0;
	virtual void exit(Mcc* light) = 0;
	virtual ~MccState() {}
};


#endif // __MCCSTATEINTERFACE_H__