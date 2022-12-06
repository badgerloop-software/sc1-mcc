#ifndef __CAN_H__
#define __CAN_H__

#include "mbed.h"

///////////////////////////////////////////////////////////////////////
/// SEE README IN "include" FOR IMPLEMENTATION AND MODIFICATION DETAILS
///////////////////////////////////////////////////////////////////////

#define MCC_OFFSET  200
#define RPM_SLOT    0
#define GPIO_SLOT   1
#define ACC_SLOT    2
#define BRK_SLOT    3

extern CANMessage outputQueue[4];
extern EventFlags queueFlags;


/// Initializes output queue id's
//  Data fields and sizes of all signals initialized in signal files
//  Returns 0 on success, -1 on failure
int initCAN(int frequency);


/// Sends CAN message from queue whenever entry present
//  Loops forever, main thread will transform into this
void CANSend();

#endif